/*
 * A347547  Number of partitions of n into 10 or more parts.
 * https://oeis.org/A347547
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) – P_{\le 9}(n) where p(n) is the total number of
 *            partitions of n and P_{\le 9}(n) counts partitions with at most
 *            9 parts. The values are computed by dynamic programming
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t MIN = 10;
    if (n < MIN)
        return 0;  /* undefined below the offset */

    /* Allocate or resize DP arrays to size n+1 */
    static seq_t *dp_total = NULL;
    static size_t dp_total_cap = 0;
    static seq_t *dp9 = NULL;
    static size_t dp9_cap = 0;
    size_t needed = (size_t)n + 1;

    if (dp_total_cap < needed) {
        free(dp_total);
        dp_total = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!dp_total) abort();
        dp_total_cap = needed;
    }
    if (dp9_cap < needed) {
        free(dp9);
        dp9 = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!dp9) abort();
        dp9_cap = needed;
    }

    /* 1. Compute total partitions p[0..n] using unlimited coin change */
    for (idx_t i = 0; i <= n; ++i)
        dp_total[i] = 0;
    dp_total[0] = 1;

    for (idx_t coin = 1; coin <= n; ++coin) {
        for (idx_t i = coin; i <= n; ++i) {
            dp_total[i] = dp_total[i] + dp_total[i - coin];
        }
    }

    /* 2. Compute partitions with parts <= 9 (i.e. at most 9 parts) */
    for (idx_t i = 0; i <= n; ++i)
        dp9[i] = 0;
    dp9[0] = 1;

    for (idx_t coin = 1; coin <= 9 && coin <= n; ++coin) {
        for (idx_t i = coin; i <= n; ++i) {
            dp9[i] = dp9[i] + dp9[i - coin];
        }
    }

    /* 3. Return the number of partitions with at least 10 parts */
    return dp_total[n] - dp9[n];
}
