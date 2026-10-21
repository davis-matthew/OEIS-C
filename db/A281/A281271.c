/*
 * A281271  Expansion of Product_{k>=1} (1 + x^(5*k-2)).
 * https://oeis.org/A281271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {5k-2 : k≥1} summing to n
 * Range:     u8 n <= 179, u16 n <= 453, u32 n <= 1303, u64 n <= 4173,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n) {
#if defined(__cplusplus)
    /* In C++ use std::vector to construct elements */
    std::vector<seq_t> dp(n + 1, (seq_t)0);
#else
    /* In C, seq_t is a plain unsigned integer */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    memset(dp, 0, (n + 1) * sizeof(seq_t));
#endif

    /* base case */
#if defined(__cplusplus)
    dp[0] = (seq_t)1;
#else
    dp[0] = 1;
#endif

    /* iterate over all available part sizes w = 5*k - 2 */
    for (idx_t k = 1; ; ++k) {
        idx_t w = 5 * k - 2;
        if (w > n) break;
        for (idx_t s = n; s >= w; --s) {
#if defined(__cplusplus)
            dp[s] += dp[s - w];
#else
            dp[s] += dp[s - w];
#endif
        }
    }

    /* retrieve result */
    seq_t result = dp[n];

#if !defined(__cplusplus)
    free(dp);
#endif
    return result;
}
