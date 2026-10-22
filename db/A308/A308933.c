/*
 * A308933  Sum of the largest parts in the partitions of n into 7 parts.
 * https://oeis.org/A308933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{m=1}^{n-6} m \times P_{6}^{\le m}(n-m) * where
 *            P_{6}^{\le m}(s) counts partitions of s into 6 parts, * each
 *            part \le m. For n < 7 the value is 0
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* No 7‑part partition below n = 7. */
    if (n < 7)
        return (seq_t)0;

    /* Allocate DP tables: dp[k][s] = number of partitions of sum s
     * into exactly k parts, each part \le current maximum.  k ranges
     * from 0 to 6.  Each table holds n+1 entries (0 … n). */
    seq_t **dp = (seq_t **)malloc(7 * sizeof(seq_t *));
    if (!dp) /* allocation failure – should not happen in test harness */ 
        return (seq_t)0;
    for (int k = 0; k <= 6; ++k)
        dp[k] = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));

    dp[0][0] = (seq_t)1;          /* one way to sum 0 with 0 parts */

    seq_t ans = (seq_t)0;

    /* Iterate over all possible values of the largest part m. */
    for (idx_t m = 1; m <= n - 6; ++m) {
        /* Update the DP with the new part value m. */
        for (idx_t s = m; s <= n; ++s) {
            for (int k = 1; k <= 6; ++k) {
                dp[k][s] += dp[k - 1][s - m];
            }
        }

        /* After inserting m, dp[6][n - m] counts partitions of n - m
         * into 6 parts each \le m.  Multiply by the largest part m
         * and add to the answer. */
        seq_t partCnt = dp[6][n - m];
        if (partCnt) {
            ans += (seq_t)m * partCnt;
        }
    }

    /* Clean up memory. */
    for (int k = 0; k <= 6; ++k)
        free(dp[k]);
    free(dp);

    return ans;
}
