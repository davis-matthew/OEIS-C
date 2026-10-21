/*
 * A284641  Expansion of (Sum_{k>=0} x^(k^2*(k+1)^2/4))^12.
 * https://oeis.org/A284641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A284641
    Formula: a(n) = coefficient of x^n in ( Σ_{k≥0} x^{k^2 (k+1)^2 / 4} )^12
    "a(n)" counts the ordered 12‑tuples of non‑negative integers
    (k₁,…,k₁₂) such that Σ (kᵢ(kᵢ+1)/2)² = n.
*/

#include <stdlib.h>
#include <string.h>   /* for memset */

/*  Memoised DP: once computed up to a maximum index, results are reused.
    The cache is recomputed only when a larger n is requested.  */
seq_t a(idx_t n)
{
    /* Static cache: array of coefficients up to the largest n requested. */
    static seq_t *coeff      = NULL;  /* coeff[i] = a(i) for 0 ≤ i ≤ max_n */
    static idx_t max_n       = 0;     /* highest index currently cached     */

    /* Recompute cache if needed. */
    if (coeff == NULL || n > max_n) {
        /* Free existing cache. */
        if (coeff != NULL) free(coeff);

        /* 1.  Generate the list of distinct exponents e_k = (k(k+1)/2)^2
            up to n.  For n≈10 000 fewer than 20 values exist.   */
        enum { MAX_EXPS = 64 };
        idx_t exps[MAX_EXPS];
        idx_t ecount = 0;

        for (idx_t k = 0; ; ++k) {
            seq_t tk = ((seq_t)k * (seq_t)(k + 1)) / 2;   /* triangular number */
            seq_t e  = tk * tk;                          /* square of triangular number */
            if (e > (seq_t)n) break;
            if (ecount >= MAX_EXPS) break;               /* safety guard */
            exps[ecount++] = (idx_t)e;
        }

        /* 2.  Dynamic programming convolution: 12‑fold self‑convolution
            of the base sequence f where f[0]=1 and f[e_k]=1.  */
        /* Allocate two working arrays. */
        seq_t *dp    = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!dp || !newdp) {   /* allocation failed – return 0 */
            if (dp) free(dp);
            if (newdp) free(newdp);
            coeff = NULL;
            max_n = 0;
            return 0;
        }

        /* Initialise dp: only the empty sum. */
        memset(dp,   0, (size_t)(n + 1) * sizeof(seq_t));
        memset(newdp, 0, (size_t)(n + 1) * sizeof(seq_t));
        dp[0] = 1;

        /* Perform 12 convolutions. */
        for (idx_t iter = 0; iter < 12; ++iter) {
            /* Zero the temporary array before each iteration. */
            memset(newdp, 0, (size_t)(n + 1) * sizeof(seq_t));

            /* Convolve with each exponent e_k. */
            for (idx_t ei = 0; ei < ecount; ++ei) {
                idx_t e = exps[ei];
                for (idx_t j = 0; j + e <= n; ++j) {
                    newdp[j + e] += dp[j];
                }
            }

            /* Swap dp and newdp for the next iteration. */
            seq_t *tmp = dp;
            dp = newdp;
            newdp = tmp;
        }

        free(newdp);          /* newdp holds the old dp array after final swap */

        /* 3.  Store the freshly computed result in the static cache. */
        coeff  = dp;
        max_n  = n;
    }

    /* Return the cached coefficient.  */
    return coeff[n];
}
