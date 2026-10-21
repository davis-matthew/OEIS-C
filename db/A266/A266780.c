/*
 * A266780  Molien series for invariants of finite Coxeter group A_11.
 * https://oeis.org/A266780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 2..12 (coefficients
 *            of 1/∏_{i=2}^{12} (1 - t^i))
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 269, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 (possible 0 elements when n == 0) */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;  /* allocation failed, return 0 as a safe default */

    /* Initialise DP array to zero */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;

    dp[0] = (seq_t)1;  /* one way to make sum 0: use no parts */

    /* For each part size from 2 to 12, update DP */
    for (idx_t part = 2; part <= 12; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
