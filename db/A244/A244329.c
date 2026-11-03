/*
 * A244329  a(n) = floor(antisigma(n) / sigma(n)) = floor(A024816(n) /
 *          A000203(n)).
 * https://oeis.org/A244329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(( n*(n+1)/2 - sigma(n) ) / sigma(n) )
 * Range:     u8 n <= 24, u16 n <= 966, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9868 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sigma(n) is the sum of positive divisors of n) */

#include <stdlib.h>

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
    seq_t sigma   = sigma_of(n);                     /* σ(n)   */
    seq_t total   = (seq_t)n * ((seq_t)n + 1) / 2;    /* n(n+1)/2 */
    seq_t antisigma = total - sigma;                 /* sum of non‑divisors  */
    return antisigma / sigma;                       /* floor division */
}
