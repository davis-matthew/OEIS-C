/*
 * A294622  Number of partitions of n into generalized octagonal numbers
 *          (A001082).
 * https://oeis.org/A294622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into generalized octagonal
 *            numbers (A001082)
 * Range:     u8 n <= 65, u16 n <= 218, u32 n <= 891, u64 n <= 4376,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for values 0..n */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return 0;                       /* allocation failure: not expected */

    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1; ; ++k) {
        /* First generalized octagonal number: g1(k) = k*(3k – 2) */
        idx_t val1 = k * (3 * k - 2);
        if (val1 > n) break;

        for (idx_t i = val1; i <= n; ++i)
            dp[(size_t)i] += dp[(size_t)(i - val1)];

        /* Second generalized octagonal number: g2(k) = k*(3k + 2) */
        idx_t val2 = k * (3 * k + 2);
        if (val2 <= n) {
            for (idx_t i = val2; i <= n; ++i)
                dp[(size_t)i] += dp[(size_t)(i - val2)];
        }
    }

    seq_t ans = dp[(size_t)n];
    free(dp);
    return ans;
}
