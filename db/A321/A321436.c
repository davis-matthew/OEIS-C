/*
 * A321436  Expansion of Product_{1 <= i <= j} (1 - x^(i^2 + j^2)).
 * https://oeis.org/A321436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 171, i16 n <= 483, i32 n <= 1511, i64 n <= 5158,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A321436: coefficient of x^n in ∏_{1≤i≤j} (1 - x^{i^2+j^2}).
   The algorithm uses dynamic programming.  For each exponent
   w = i^2 + j^2 we update dp[m] -= dp[m-w] for all m ≥ w.
   The table dp[n] is built once to the maximum index requested
   and reused for subsequent calls.  Complexity is O(n·√n). */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *dp      = NULL;  /* array of coefficients */
    static idx_t  dp_size = 0;     /* current allocated size */

    if (dp_size <= n) {
        /* allocate new table */
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_dp) return 0;  /* Out of memory → return 0 as safe */
        /* zero‑initialize the array */
        memset(new_dp, 0, (size_t)new_size * sizeof(seq_t));
        /* first coefficient is 1 */
        new_dp[0] = 1;

        /* DP over all weights w = i^2 + j^2 with 1 ≤ i ≤ j and w ≤ n */
        for (idx_t i = 1; i * i <= n; ++i) {
            idx_t ii = i * i;
            for (idx_t j = i; ; ++j) {
                idx_t jj = j * j;
                idx_t w  = ii + jj;
                if (w > n) break;
                for (idx_t m = n; m >= w; --m)
                    new_dp[m] -= new_dp[m - w];
            }
        }

        /* free old table if it existed */
        if (dp) free(dp);
        dp      = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
