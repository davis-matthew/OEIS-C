/*
 * A394792  Number of positive integers <= n with exactly three odd divisors.
 * https://oeis.org/A394792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p odd prime, p² ≤ n} (⌊log₂(n / p²)⌋ + 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   build_primes_upto, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, free */
#include <stdint.h>          /* uint64_t */
#include <stddef.h>          /* size_t */
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 9) {
        /* The smallest integer with 3 odd divisors is 9 (3²). */
        return 0;
    }

    /* compute sqrt(n) once, used as upper bound for primes */
    idx_t lim = int_sqrt(n);

    /* primes array returned by build_primes_upto */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(lim, &primes, &prime_cnt);
    if (!primes || prime_cnt == 0) {
        /* no primes found; should not happen for n ≥ 9 */
        return 0;
    }

    seq_t total = 0;

    /* iterate over primes, skipping the even prime 2 */
    for (size_t i = 1; i < prime_cnt; ++i) {          /* start at i=1 to skip 2 */
        uint64_t p = primes[i];
        /* p² may be computed safely in 64‑bit: p ≤ sqrt(n) ≤ 2³²⁻¹, so p² fits */
        idx_t p2 = (idx_t)p * (idx_t)p;
        if (p2 > n) break;                /* further primes are larger */

        idx_t rem = n / p2;                /* floor division, rem ≥ 1 */

        /* compute floor(log₂(rem)) */
        idx_t log2 = 0;
        idx_t t = rem;
        while (t >> 1) {
            log2++;
            t >>= 1;
        }

        /* add number of eligible e: 0 .. log₂(rem) inclusive */
        total += (seq_t)(log2 + 1);
    }

    free(primes);
    return total;
}
