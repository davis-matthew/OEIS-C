/*
 * A278883  a(n) = (4*n+1) * ( binomial(3*n,n)/(2*n+1) )^2.
 * https://oeis.org/A278883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+1) * ( binomial(3*n,n)/(2*n+1) )^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t binom = binomial(3 * n, n);
    seq_t denom = (seq_t)(2 * n + 1);
    seq_t q = binom / denom;          /* division is exact */
    seq_t sq = q * q;
    seq_t mult = (seq_t)(4 * n + 1);
    return mult * sq;
}
