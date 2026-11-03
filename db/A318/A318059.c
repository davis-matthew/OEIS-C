/*
 * A318059  a(n) is the numerator of sigma(sigma(n))/n.
 * https://oeis.org/A318059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the numerator of σ(σ(n))/n, where σ is the
 *            sum-of-divisors function
 * Range:     u8 n <= 53, u16 n <= 9627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9628 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* σ(n) */
    seq_t sigma1 = sigma_of(n);
    /* σ(σ(n)) – σ(σ(n)) will fit in idx_t for the ranges of interest */
    uint64_t sigma1_u = (uint64_t)sigma1;
    seq_t sigma2 = sigma_of(sigma1_u);

    /* Reduce the fraction sigma2 / n to lowest terms */
    seq_t g = gcd((seq_t)n, sigma2);
    return sigma2 / g;
}
