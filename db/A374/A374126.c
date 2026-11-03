/*
 * A374126  a(n) = gcd(sigma(n), sopfr(n)), where sigma is the sum of
 *          divisors, and sopfr is the sum of prime factors with repetition.
 * https://oeis.org/A374126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), sopfr(n))
 * Range:     u8 n <= 1589, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 118 terms through n = 9714 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* Compute sopfr(n): sum of prime factors with repetition */
    seq_t sopfr = 0;
    idx_t m = n;
    if (m > 1) {
        /* Handle factor 2 separately */
        if ((m & 1) == 0) {
            idx_t exp = 0;
            while ((m & 1) == 0) {
                m >>= 1;
                exp++;
            }
            sopfr += (seq_t)2 * (seq_t)exp;
        }
        /* Check odd factors */
        for (idx_t p = 3; p * p <= m; p += 2) {
            if (m % p == 0) {
                idx_t exp = 0;
                while (m % p == 0) {
                    m /= p;
                    exp++;
                }
                sopfr += (seq_t)p * (seq_t)exp;
            }
        }
        /* Remaining prime factor > 1 */
        if (m > 1) {
            sopfr += (seq_t)m;
        }
    }

    /* Return gcd(sigma, sopfr) */
    return gcd(sigma, sopfr);
}
