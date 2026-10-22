/*
 * A326846  Length times maximum of the integer partition with Heinz number n.
 * https://oeis.org/A326846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(n) * max_index_of_prime_factor_of(n)
 * Range:     u8 n <= 1437, u16 n <= 8556, u32 n <= 8556, u64 n <= 8556,
 *            u128 n <= 8556, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here Ω(n) = total number of prime factors of n counted with multiplicity. */
/* The max_index is the 1‑based rank of the greatest prime dividing n.   */

#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* keep a global list of primes up to the maximum n seen so far            */
    static idx_t max_limit = 0;          /* current upper bound of primes   */
    static uint64_t *primes = NULL;      /* array of primes                 */
    static size_t prime_cnt = 0;         /* number of primes in array       */

    /* enlarge the prime list if the current n is larger than the existing   */
    if (n > max_limit) {
        if (primes) {
            free(primes);
            primes = NULL;
            prime_cnt = 0;
        }
        max_limit = (idx_t)n;
        build_primes_upto(max_limit, &primes, &prime_cnt);
    }

    /* factorise n using the helper routine --------------------------------- */
    uint64_t pf[64], expf[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pf, expf, &cnt);

    /* compute Ω(n) and max prime index ------------------------------------ */
    seq_t omega = 0;
    seq_t max_index = 0;
    for (size_t i = 0; i < cnt; ++i) {
        omega += (seq_t)expf[i];

        /* binary search for the index of pf[i] in the prime list              */
        size_t lo = 0, hi = prime_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (primes[mid] == pf[i]) {
                lo = mid;
                break;
            } else if (primes[mid] < pf[i]) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        seq_t idx = (seq_t)(lo + 1);   /* primes are 1‑based indexed    */
        if (idx > max_index)
            max_index = idx;
    }

    return omega * max_index;
}
