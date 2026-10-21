/*
 * A287997  Expansion of 1/((1-x)(1-x^3)(1-x^5) ... (1-x^13)).
 * https://oeis.org/A287997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 112, u32 n <= 840, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of partitions of n into odd parts ≤ 13.
   This is computed by the classic coin‑change DP where the coins are
   1,3,5,7,9,11,13.  The recurrence is
       dp[0] = 1;
       dp[s] += dp[s - coin]   (for each coin in 1,3,5,7,9,11,13)
   and the result is a(n) = dp[n].
*/

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Odd part limits */
    static const idx_t parts[] = {1, 3, 5, 7, 9, 11, 13};
    static const size_t PARTS = 7;

    /* Allocate DP array of size n+1. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;         /* OOM guard */

    /* Initialize DP table. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Coin‑change DP. */
    for (size_t p = 0; p < PARTS; ++p) {
        idx_t coin = parts[p];
        for (idx_t s = coin; s <= n; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
