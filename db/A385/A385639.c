/*
 * A385639  a(n) = Sum_{k=0..n} binomial(4*n+1,k) * binomial(2*n-k,n-k).
 * https://oeis.org/A385639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(4*n+1,k) * binomial(2*n-k,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* pre‑compute constants */
    const idx_t amax = 4 * n + 1;   /* 4*n+1 */
    const idx_t bmax = 2 * n;       /* 2*n   */

    /* start values of the two binomial sequences */
    seq_t c1 = 1;                                 /* binom(4*n+1,0)   */
    seq_t c2 = binom(2 * n, n);                  /* binom(2*n,   n)  */

    seq_t sum = 0;

    /* k = 0 term is already in c1 and c2 */
    sum += c1 * c2;

    for (idx_t k = 1; k <= n; ++k) {
        /* update binomial(4*n+1, k) from binomial(4*n+1, k-1) */
        /* c1 = c1 * (amax - (k-1)) / k */
        c1 = c1 * (amax - (k - 1)) / k;

        /* update binomial(2*n-k, n-k) from the previous term
           using: c2_{k} = c2_{k-1} * (n - (k-1)) / (bmax - (k-1))        */
        c2 = c2 * (n - (k - 1)) / (bmax - (k - 1));

        /* add the current term to the sum */
        sum += c1 * c2;
    }

    return sum;
}
