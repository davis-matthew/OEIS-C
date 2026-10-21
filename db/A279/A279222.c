/*
 * A279222  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)*(4*k-1)/6)).
 * https://oeis.org/A279222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into hexagonal pyramidal
 *            numbers (A002412). This is equivalent to the coefficient of x^n
 *            in the generating function Π_{k≥1} 1/(1 - x^{k*(k+1)*(4*k-1)/6})
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for all values up to n */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                /* allocation failure – return 0 */

    /* Initialize DP table */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                        /* one way to partition 0 */

    /* Process each hexagonal pyramidal number as a coin */
    for (idx_t k = 1;; ++k) {
        /* Compute h_k = k*(k+1)*(4*k-1)/6 using seq_t arithmetic to avoid overflow */
        seq_t ck = (seq_t)k * (k + 1) * (4 * k - 1) / 6;
        idx_t coin = (idx_t)ck;
        if (coin > n) break;           /* no larger coins needed */

        /* Unbounded knapsack (coin change) update */
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
