/*
 * A323208  a(n) = hypergeometric([-n - 1, n + 2], [-n - 2], n).
 * https://oeis.org/A323208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n+1} binomial(n+1+j, n+1)*(1 - j/(n+2))*n^j
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute binomial(n+1+(n+1), n+1) = binomial(2n+2, n+1) */
    seq_t binom = 1;
    idx_t limit = n + 1;
    for (idx_t t = 1; t <= limit; ++t)
    {
        binom = (binom * ((seq_t)(n + 1 + t))) / (seq_t)t;
    }

    /* Horner evaluation: a(n) = Σ_{j=0}^{n+1} C_j * n^j,
       where C_j = binomial(n+1+j, n+1)*(n+2-j)/(n+2) */
    seq_t result = 0;
    for (idx_t j = limit; ; --j)
    {
        seq_t coef = binom * ((seq_t)(n + 2 - j));
        coef = coef / ((seq_t)(n + 2));
        result = result * ((seq_t)n) + coef;
        if (j == 0) break;
        binom = (binom * (seq_t)j) / ((seq_t)(n + 1 + j));
    }
    return result;
}
