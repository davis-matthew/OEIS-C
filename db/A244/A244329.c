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
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sigma(n) is the sum of positive divisors of n) */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma   = sigma_of(n);                     /* σ(n)   */
    seq_t total   = (seq_t)n * ((seq_t)n + 1) / 2;    /* n(n+1)/2 */
    seq_t antisigma = total - sigma;                 /* sum of non‑divisors  */
    return antisigma / sigma;                       /* floor division */
}
