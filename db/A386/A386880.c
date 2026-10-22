/*
 * A386880  a(n) = [x^n] 1/(1 - x)^(n*(n+1)/2).
 * https://oeis.org/A386880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0: a(n)=binomial(n*(n+3)/2, n) * (n+1)/(n+3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* k = n*(n+3)/2 */
    idx_t k = n * (n + 3) / 2;

    /* bin = binomial(k, n) */
    seq_t bin = binomial(k, n);

    /* compute bin * (n+1) / (n+3) */
    seq_t numerator   = bin * ((seq_t)n + (seq_t)1);
    seq_t denominator = (seq_t)n + (seq_t)3;

    return numerator / denominator;
}
