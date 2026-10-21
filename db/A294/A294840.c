/*
 * A294840  Expansion of Product_{k>=1} (1 + x^(2*k-1))^(k*(5*k-3)/2)*(1 +
 *          x^(2*k))^(k*(5*k+3)/2).
 * https://oeis.org/A294840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in the series expansion of *
 *            Π_{k>=1} (1 + x^(2k-1))^(k*(5k-3)/2) * (1 + x^(2k))^(k*(5k+3)/2)
 *            * * This implements the product Σ_{m>=1} (1 + x^m)^{e(m)} with *
 *            e(m) = { ((m+1)/2) * (5*(m+1)/2 - 3) / 2, m odd * { (m/2) *
 *            (5*(m/2) + 3) / 2, m even * * The function retu...
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate two arrays of size n+1 */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) {
        /* memory allocation failed; return 0 as safe fallback */
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;   /* start with empty product */

    /* Iterate over part sizes m = 1 .. n */
    for (idx_t m = (idx_t)1; m <= n; ++m)
    {
        /* compute exponent e(m) */
        idx_t e;
        if (m & 1u) { /* m odd */
            idx_t k = (m + (idx_t)1) >> 1;      /* k = (m+1)/2 */
            e = k * ((idx_t)5 * k - (idx_t)3) >> 1;   /* divide by 2 */
        } else { /* m even */
            idx_t k = m >> 1;                    /* k = m/2 */
            e = k * ((idx_t)5 * k + (idx_t)3) >> 1;   /* divide by 2 */
        }

        /* maximum copies of this part that could fit into n */
        idx_t max_j = n / m;
        idx_t j_limit = (e < max_j) ? e : max_j;

        /* reset newdp */
        for (size_t i = 0; i < sz; ++i)
            newdp[i] = (seq_t)0;

        /* binomial coefficient C(e, j) iteratively */
        seq_t coeff = (seq_t)1;   /* C(e,0) */

        for (idx_t j = (idx_t)0; j <= j_limit; ++j)
        {
            idx_t shift = j * m;
            size_t sh = (size_t)shift;    /* shift index */

            /* add contributions of this term to all positions >= shift */
            for (size_t t = sh; t < sz; ++t)
            {
                /* dp[t - sh] * coeff added to newdp[t] */
                newdp[t] += dp[t - sh] * coeff;
            }

            /* prepare next binomial coefficient */
            if (j < j_limit)
            {
                /* coeff = coeff * (e - j) / (j + 1) */
                seq_t num = coeff * (seq_t)(e - j);
                coeff = num / (seq_t)(j + (idx_t)1);
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(newdp);
    return result;
}
