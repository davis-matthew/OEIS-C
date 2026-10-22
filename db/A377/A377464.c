/*
 * A377464  Number of connected pairs of subsets of [n] with each being a
 *          different size.
 * https://oeis.org/A377464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n-2} binomial(n,i) * Sum_{j=i+1..n-1}
 *            (binomial(n,j) - binomial(i,n-j))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Precompute C(n,k) for k = 0..n */
    seq_t *cn = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    cn[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        cn[k] = cn[k - 1] * (seq_t)(n - k + 1) / (seq_t)k;

    /* Suffix sums of C(n,k) for k = 0..n-1 (C(n,n) not used) */
    seq_t *suff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    suff[n] = (seq_t)0;
    for (idx_t k = n; k > 0; --k)
        suff[k - 1] = suff[k] + cn[k - 1];

    /* Arrays for current row of Pascal's triangle (C(i,k)) and its prefix sums */
    seq_t *row = (seq_t *)calloc(n, sizeof(seq_t));   /* zeros, row[0] will be set */
    seq_t *pre = (seq_t *)calloc(n, sizeof(seq_t));

    row[0] = (seq_t)1;
    pre[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    for (idx_t i = 0; i <= n - 2; ++i)
    {
        seq_t cn_i = cn[i];

        idx_t r = (i < (n - i - 1)) ? i : (n - i - 1);
        seq_t sumC = pre[r] - (seq_t)1;          /* sum_{k=1}^{r} C(i,k) */
        seq_t S     = suff[i + 1] - sumC;        /* sum_{j=i+1}^{n-1} (C(n,j)) - sumC */

        result += cn_i * S;

        /* Prepare next row of binomial coefficients (i+1) */
        if (i < n - 2) {
            idx_t new_i = i + 1;
            for (idx_t k = new_i; k > 0; --k)
                row[k] = row[k] + row[k - 1];     /* Pascal update in place */

            pre[0] = row[0];
            for (idx_t k = 1; k <= new_i; ++k)
                pre[k] = pre[k - 1] + row[k];
        }
    }

    free(cn);
    free(suff);
    free(row);
    free(pre);

    return result;
}
