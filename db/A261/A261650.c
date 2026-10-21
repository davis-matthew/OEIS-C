/*
 * A261650  Expansion of Product_{k>=0} ((1+x^(4*k+1))/(1-x^(4*k+1)))^2.
 * https://oeis.org/A261650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} ((1 + x^(4k+1))/(1 -
 *            x^(4k+1)))^2
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The factor for each part size m = 4k+1 is
   1 + Σ_{j≥1} 4j x^(j*m) .
   We perform a DP over all such m up to n, storing the coefficients
   modulo the underlying seq_t type.  The recurrence is
     dp_new[n] = dp[n] + Σ_{j=1}^{⌊n/m⌋} 4j * dp[n – j*m] .
   The inner sum is evaluated in O(1) per position using prefix sums. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate two working arrays of length n+1. */
    seq_t *dp    = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));

    /* Initialise dp[0] = 1, others 0. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process all part sizes m = 1,5,9,… up to n. */
    for (idx_t m = 1; m <= n; m += 4) {
        /* Start with newdp = dp (j = 0 case). */
        for (idx_t i = 0; i <= n; ++i)
            newdp[i] = dp[i];

        /* Update contributions of j ≥ 1 for each residue class modulo m. */
        for (idx_t r = 0; r < m; ++r) {
            if (r > n) break;   /* skip empty classes when m > n */

            seq_t pref_sum   = 0;   /* Σ_{t=0}^{k-1} dp[r + t*m]   */
            seq_t pref_weight= 0;   /* Σ_{t=0}^{k-1} t*dp[r + t*m] */
            idx_t k = 0;

            for (idx_t pos = r; pos <= n; pos += m, ++k) {
                seq_t val = dp[pos];        /* value from previous dp */

                if (k > 0) {
                    /* add contribution of using j = k copies of size m */
                    seq_t add = 4 * ((seq_t)k * pref_sum - pref_weight);
                    newdp[pos] += add;
                }

                pref_sum   += val;
                pref_weight += (seq_t)k * val;
            }
        }

        /* Prepare for next part size: swap dp and newdp. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* Free temporary buffers. */
    free(dp);
    free(newdp);

    return result;
}
