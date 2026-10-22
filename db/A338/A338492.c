/*
 * A338492  Least number of centered heptagonal numbers needed to represent n.
 * https://oeis.org/A338492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A338492 – Least number of centered heptagonal numbers needed to represent n.
 * Implementation: Unbounded coin‑change dynamic programming.
 *   Coins are C7(k) = 1 + 7*k*(k-1)/2 for k >= 1.
 *   For each 1 ≤ n, dp[n] is the minimal count of such coins summing to n.
 *   Complexity: O(n * h) where h is the number of coins ≤ n (≈ 170 for n ≤ 10000),
 *   well under the time limit.
 *   No external utilities are required.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial case (unused in OEIS offset but harmless). */
    if (n == 0)
        return (seq_t)0;

    /* dp[i] holds the minimal number of coins for sum i. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* allocation failure → return 0 */

    dp[0] = (seq_t)0;
    /* Worst case: use all 1‑coins. */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)i;

    /* Generate all centered heptagonal numbers ≤ n. */
    idx_t max_coins = n + 1;               /* safe upper bound */
    idx_t *coins = (idx_t *)malloc((size_t)max_coins * sizeof(idx_t));
    if (!coins) {
        free(dp);
        return (seq_t)0;
    }
    idx_t coin_cnt = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t val = 1 + (7 * k * (k - 1)) / 2;
        if (val > n)
            break;
        coins[coin_cnt++] = val;
    }

    /* Unbounded knapsack DP */
    for (idx_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t s = coin; s <= n; ++s) {
            seq_t cand = dp[s - coin] + (seq_t)1;
            if (cand < dp[s])
                dp[s] = cand;
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(coins);
    return result;
}
