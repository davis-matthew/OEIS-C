/*
 * A298987  a(n) = [x^n] Product_{k>=1} (1 + n*x^k)^k.
 * https://oeis.org/A298987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in product_{k>=1} (1 + n*x^k)^k
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* size of coefficient array */
    size_t sz = (size_t)n + 1;

#ifdef __cplusplus
    std::vector<seq_t> dp(sz, (seq_t)0);
    std::vector<seq_t> newdp(sz, (seq_t)0);
    dp[0] = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxj = n / k;
        if (maxj > k) maxj = k;
        std::fill(newdp.begin(), newdp.end(), (seq_t)0);

        seq_t w = 1;
        for (idx_t j = 0; j <= maxj; ++j) {
            idx_t offset = k * j;
            for (size_t m = offset; m < sz; ++m)
                newdp[m] = newdp[m] + dp[m - offset] * w;

            if (j == maxj) break;
            w = w * (seq_t)n * (seq_t)(k - j);
            w = w / (seq_t)(j + 1);
        }
        dp.swap(newdp);
    }
    return dp[n];
#else
    seq_t *dp    = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp || !newdp) return (seq_t)0;
    for (size_t i = 0; i < sz; ++i) dp[i] = (seq_t)0;
    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxj = n / k;
        if (maxj > k) maxj = k;

        for (size_t i = 0; i < sz; ++i) newdp[i] = (seq_t)0;
        seq_t w = 1;
        for (idx_t j = 0; j <= maxj; ++j) {
            idx_t offset = k * j;
            for (size_t m = offset; m < sz; ++m)
                newdp[m] += dp[m - offset] * w;

            if (j == maxj) break;
            w = w * (seq_t)n * (seq_t)(k - j);
            w = w / (seq_t)(j + 1);
        }
        /* swap buffers */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
#endif
}
