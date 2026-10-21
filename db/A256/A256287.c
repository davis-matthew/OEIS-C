/*
 * A256287  Number of partitions of 7n into 7 parts.
 * https://oeis.org/A256287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 7*n into exactly 7 parts
 *            (equivalently the number of non‑negative integer solutions to
 *            1*a1 + 2*a2 + … + 7*a7 = 7*n – 7). a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* target sum after subtracting 1 from each of the 7 parts */
    idx_t target_idx = (idx_t)(7 * n - 7);
    size_t target = (size_t)target_idx;

#if defined(__cplusplus)
    std::vector<seq_t> dp(target + 1, 0);
    dp[0] = (seq_t)1;
    for (int coin = 1; coin <= 7; ++coin) {
        for (size_t s = coin; s <= target; ++s)
            dp[s] += dp[s - coin];
    }
    return dp[target];
#else
    /* C implementation */
    seq_t *dp = (seq_t *)calloc(target + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* fallback on allocation failure */
    dp[0] = (seq_t)1;
    for (int coin = 1; coin <= 7; ++coin) {
        for (size_t s = coin; s <= target; ++s)
            dp[s] = dp[s] + dp[s - coin];
    }
    seq_t result = dp[target];
    free(dp);
    return result;
#endif
}
