/*
 * A298859  Number of partitions of n into distinct fourth powers.
 * https://oeis.org/A298859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct fourth powers
 * Range:     u8 n <= 80, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic programming: dp[i] = #ways to write i as a sum of distinct k^4 */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp)
        return 0;  /* unlikely to happen for n <= 10000 */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1;; ++k) {
        seq_t p = (seq_t)k;
        p *= p; /* k^2 */
        p *= p; /* k^4 */
        if (p > (seq_t)n)
            break;

        idx_t up = (idx_t)p;
        for (idx_t i = n; i >= up; --i) {
            dp[i] += dp[i - up];
        }
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
