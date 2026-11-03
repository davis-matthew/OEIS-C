/*
 * A325636  a(n) = gcd(2n, sigma(n)).
 * https://oeis.org/A325636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(2n, sigma(n))
 * Range:     u8 n <= 208, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9981 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = gcd(2n, sum of divisors of n) using helper utilities. */

#include <stdlib.h>
#include <stdint.h>
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
    /* Ensure n is at least 1; behavior for n == 0 is undefined but allowed. */
    seq_t sigma_n = sigma_of(n);
    seq_t two_n   = (seq_t)n * 2;
    return gcd(sigma_n, two_n);
}
