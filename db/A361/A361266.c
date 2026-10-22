/*
 * A361266  Multiplicative with a(p^e) = p^(e + 3), e > 0.
 * https://oeis.org/A361266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * Π_{p|n} p³ (multiplicative: a(p^e) = p^(e+3))
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* n = 1 gives the unit value */
    if (n == 1) return (seq_t)1;

    seq_t rad = 1;               /* product of distinct prime divisors of n */
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        /* obtain the smallest prime factor of the current m */
        uint64_t p = _smallest_prime_factor(m);

        /* multiply rad once for this distinct prime */
        rad *= (seq_t)p;

        /* strip all powers of p from m */
        while (m % p == 0) {
            m /= p;
        }
    }

    /* compute rad³ = rad² * rad */
    seq_t rad_sq     = rad * rad;
    seq_t rad_cubed  = rad_sq * rad;

    /* return n * rad³ */
    return (seq_t)n * rad_cubed;
}
