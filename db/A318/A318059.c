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
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

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
