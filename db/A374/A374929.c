/*
 * A374929  Expansion of Sum_{1<=i<=j} q^(i+j)/( (1-q^i)*(1-q^j) )^2.
 * https://oeis.org/A374929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*sigma_3(n) - (6*n+1)*sigma(n))/24
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A374929. Requires util functions sigma_of() and sigma_k(). */

#include "../utils/sigma_of.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence starts at a(2) */

    seq_t sigma1 = sigma_of(n);   /* σ(n) */
    seq_t sigma3 = sigma_k(n, 3); /* σ₃(n) */

    seq_t n_val = (seq_t)n;
    seq_t term1 = 7 * sigma3;
    seq_t term2 = (6 * n_val + 1) * sigma1;
    seq_t numerator = term1 - term2;

    return numerator / (seq_t)24;
}
