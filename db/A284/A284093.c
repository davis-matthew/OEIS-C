/*
 * A284093  Expansion of Product_{k>=1} (1 + x^(8*k-1)).
 * https://oeis.org/A284093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            8*k-1
 * Range:     u8 n <= 298, u16 n <= 743, u32 n <= 2130, u64 n <= 6800,
 *            u128 n <= 9842, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the sequence value for index n. */
seq_t a(idx_t n)
{
    /* Allocate array for dynamic programming.  Use a cast to satisfy C++ builds. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) abort();

    /* Initialize DP array.  dp[0] = 1 (empty partition). */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* For each allowed part 8*k-1, perform the classic 0/1 knapsack DP. */
    for (idx_t k = 1;; ++k) {
        idx_t part = 8 * k - 1;
        if (part > n) break;
        for (idx_t i = n; i >= part; --i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
