/*
 * A350649  Number of partitions of [n] avoiding blocks containing their own
 *          index when blocks are ordered with decreasing largest elements.
 * https://oeis.org/A350649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of [n] avoiding blocks containing
 *            their own index when blocks are ordered with decreasing largest
 *            elements
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Use C++ vector for arbitrary‑precision seq_t */
    std::vector<seq_t> dp(n + 1, 0), ndp(n + 1, 0);
    dp[0] = 1;
    idx_t max_blocks = 0;
    for (idx_t x = n; x >= 1; --x) {
        std::fill(ndp.begin(), ndp.begin() + max_blocks + 2, (seq_t)0);
        for (idx_t k = 0; k <= max_blocks; ++k) {
            seq_t val = dp[k];
            if (!val) continue;
            idx_t choices = (x <= k) ? k - 1 : k;
            if (choices > 0) ndp[k] += val * (seq_t)choices;
            if (x != k + 1) ndp[k + 1] += val;
        }
        dp.swap(ndp);
        ++max_blocks;
    }
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) total += dp[k];
    return total;
#else
    /* For C, use static arrays for n up to 10000, dynamic otherwise */
    if (n > 10000) {
        seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        seq_t *ndp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        dp[0] = 1;
        idx_t max_blocks = 0;
        for (idx_t x = n; x >= 1; --x) {
            memset(ndp, 0, (size_t)(max_blocks + 2) * sizeof(seq_t));
            for (idx_t k = 0; k <= max_blocks; ++k) {
                seq_t val = dp[k];
                if (!val) continue;
                idx_t choices = (x <= k) ? k - 1 : k;
                if (choices > 0) ndp[k] = ndp[k] + val * (seq_t)choices;
                if (x != k + 1) ndp[k + 1] = ndp[k + 1] + val;
            }
            seq_t *tmp = dp; dp = ndp; ndp = tmp;
            ++max_blocks;
        }
        seq_t total = 0;
        for (idx_t k = 0; k <= n; ++k) total += dp[k];
        free(dp); free(ndp);
        return total;
    } else {
        static seq_t dp[10001];
        static seq_t ndp[10001];
        memset(dp, 0, sizeof(dp));
        memset(ndp, 0, sizeof(ndp));
        dp[0] = 1;
        idx_t max_blocks = 0;
        for (idx_t x = n; x >= 1; --x) {
            for (idx_t i = 0; i <= max_blocks + 1; ++i) ndp[i] = 0;
            for (idx_t k = 0; k <= max_blocks; ++k) {
                seq_t val = dp[k];
                if (!val) continue;
                idx_t choices = (x <= k) ? k - 1 : k;
                if (choices > 0) ndp[k] = ndp[k] + val * (seq_t)choices;
                if (x != k + 1) ndp[k + 1] = ndp[k + 1] + val;
            }
            for (idx_t i = 0; i <= max_blocks + 1; ++i) dp[i] = ndp[i];
            ++max_blocks;
        }
        seq_t total = 0;
        for (idx_t k = 0; k <= n; ++k) total += dp[k];
        return total;
    }
#endif
}
