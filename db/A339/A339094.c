/*
 * A339094  Number of (unordered) ways of making change for n US Dollars using
 *          the current US denominations of $1, $2, $5, $10, $20, $50 and $100
 *          bills.
 * https://oeis.org/A339094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of (unordered) ways of making change for n US
 *            Dollars using the denominations 1,2,5,10,20,50,100
 * Range:     u8 n <= 41, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case for n==0 */
    if (n == 0)
        return (seq_t)1;

    /* Dynamic programming array: dp[i] = number of ways to make i */
    /* Allocate (n+1) elements of type seq_t, zero-initialized */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;      /* Failure doesn't affect correctness */

    dp[0] = (seq_t)1;              /* One way to make 0: use no bills */

    /* Denominations */
    const idx_t coins[] = {1, 2, 5, 10, 20, 50, 100};
    const size_t num_coins = sizeof(coins) / sizeof(coins[0]);

    for (size_t c = 0; c < num_coins; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
