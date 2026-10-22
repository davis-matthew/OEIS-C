/*
 * A347304  a(n) = n!/(floor(n/2)!*floor(n/3)!*floor(n/6)!).
 * https://oeis.org/A347304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!/(floor(n/2)! * floor(n/3)! * floor(n/6)!)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0) return (seq_t)1;

    idx_t a1 = n / 2;
    idx_t b1 = n / 3;
    idx_t c1 = n / 6;

    /* Obtain primes up to n once per call */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes(n, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        /* Compute exponent of prime p in the multinomial coefficient */
        idx_t exp = 0;
        idx_t d = p;
        while (d <= n)   { exp += n   / d; d *= p; }
        d = p;
        while (d <= a1)  { exp -= a1 / d; d *= p; }
        d = p;
        while (d <= b1)  { exp -= b1 / d; d *= p; }
        d = p;
        while (d <= c1)  { exp -= c1 / d; d *= p; }

        /* Multiply p^exp into result */
        for (idx_t e = 0; e < exp; ++e) {
            result *= (seq_t)p;
        }
    }

    free(primes);
    return result;
}
