/*
 * A344793  a(n) is the number of primes of the form prime(n) + 2*q where q <
 *          prime(n) is prime.
 * https://oeis.org/A344793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of primes of the form prime(n) + 2*q * where q
 *            < prime(n) is prime. Computed by * generating all primes ≤
 *            prime(n) and testing each candidate
 * Range:     u8 n <= 1871, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Prime at index n (1‑based).  get_prime returns uint32_t.  */
    uint32_t p = get_prime(n);

    /* Generate all primes ≤ p.  */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)p, &primes, &prime_cnt);

    seq_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = primes[i];
        if (q >= p) {
            break; /* only primes q < p are allowed */
        }
        uint64_t cand = (uint64_t)p + 2ULL * q;
        if (is_prime((idx_t)cand)) {
            ++cnt;
        }
    }

    free(primes);
    return cnt;
}
