/*
 * A249792  G.f.: Sum_{n>=0} x^n/(1-x)^(4*n) * Sum_{k=0..n} C(n,k)^2 * x^k.
 * https://oeis.org/A249792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{i=0}^n \sum_{j=0}^i \binom(i,j)^2
 *            \binom(n+3i-j-1,\;4i-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t bi   = (seq_t)1;                       /* \binom(i,0) */
        idx_t k_top = n + 3 * i - 1;                 /* top of binomial for j=0 */
        idx_t c      = 4 * i - 1;                    /* bottom of binomial */
        seq_t bk     = binom(k_top, c);              /* \binom(k_top,c) */

        for (idx_t j = 0; j <= i; ++j)
        {
            /* term = binom(i,j)^2 * binom(n+3i-j-1,4i-1) */
            seq_t term = bi * bi;
            term *= bk;
            result += term;

            /* update for next j */
            if (j == i) break;

            /* bi -> binom(i,j+1) */
            bi = bi * (seq_t)(i - j);
            bi = bi / (seq_t)(j + 1);

            /* bk -> binom(k_top - (j+1), c) from binom(k_top - j, c) */
            seq_t denom = (seq_t)(k_top - j);
            bk = bk * (seq_t)(k_top - j - c);
            bk = bk / denom;
        }
    }

    return result;
}
