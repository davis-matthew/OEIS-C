/*
 * A280423  G.f.: Product_{k>=1} (1 + x^(k*(k+1))) / (1 - x^k).
 * https://oeis.org/A280423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S subset of {k | k(k+1) <= n}} p(n - sum_{k in S}
 *            k(k+1)) (where p(m) is the ordinary partition function). This is
 *            implemented by dynamic programming: for each k we first add the
 *            unbounded part k to the partitions (standard partition DP), then
 *            optionally include the special part of si...
 * Range:     u8 n <= 14, u16 n <= 38, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate DP array: dp[m] = number of ways to sum to m after processing
       parts up to current k, including any subset of special parts processed. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* fallback on allocation failure */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Unbounded part of size k: standard partition DP update */
        for (idx_t m = k; m <= n; ++m) {
            dp[m] += dp[m - k];
        }

        /* Special part of size t = k*(k+1). Use 0-1 knapsack update. */
        seq_t t = (seq_t)k * (k + 1);
        if ((idx_t)t <= n) {
            for (idx_t m = n; m >= (idx_t)t; --m) {
                dp[m] += dp[m - (idx_t)t];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
