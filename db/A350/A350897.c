/*
 * A350897  Number of partitions of n such that 5*(smallest part) = (number of
 *          parts).
 * https://oeis.org/A350897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s>=1, 5*s^2 <= n} p(n-5*s^2, <= 5*s-1) where p(m, <=
 *            t) is the number of partitions of m into at most t parts
 * Range:     u8 n <= 30, u16 n <= 72, u32 n <= 205, u64 n <= 658,
 *            u128 n <= 2293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine maximal s such that 5*s^2 <= n */
    idx_t smax = 0;
    for (idx_t s = 1;; ++s) {
        unsigned __int128 val = (unsigned __int128)5 * s * s;
        if (val > n) break;
        smax = s;
    }
    if (smax == 0) return 0;                          /* n < 5 -> no solution */

    idx_t mmax = 5 * smax;                            /* m = 5*s       */
    idx_t kmax = mmax - 1;                            /* need partitions <= 5*s-1 */

    size_t rows = (size_t)(n + 1);
    size_t cols = (size_t)(kmax + 1);
    size_t total = rows * cols;

    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return 0;                                 /* allocation failed */

    /* Base: partitions of 0 are 1 for any number of parts */
    for (size_t k = 0; k < cols; ++k)
        dp[0 * cols + k] = 1;

    /* DP: p(n, <= k) = p(n, <= k-1) + p(n-k, <= k)   if n >= k
             otherwise p(n, <= k) = p(n, <= k-1)           */
    for (size_t i = 1; i < rows; ++i) {
        dp[i * cols + 0] = 0;
        for (size_t k = 1; k < cols; ++k) {
            seq_t val = dp[i * cols + (k - 1)];
            if (i >= k) val += dp[(i - k) * cols + k];
            dp[i * cols + k] = val;
        }
    }

    seq_t ans = 0;
    for (idx_t s = 1; s <= smax; ++s) {
        unsigned __int128 sub128 = (unsigned __int128)5 * s * s;
        idx_t N = n - (idx_t)sub128;
        idx_t k = 5 * s - 1;                          /* <= 5*s-1 parts */
        ans += dp[N * cols + k];
    }

    free(dp);
    return ans;
}
