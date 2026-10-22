/*
 * A339279  Number of partitions of 3*n into powers of 3 where every part
 *          appears at least 2 times.
 * https://oeis.org/A339279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 3*n into powers of 3 (1, 3, 9, …)
 *            where every part that appears is used at least twice. The
 *            generating function is ∏_{k≥0}(1 + x^{2·3^k}/(1 - x^{3^k})).
 *            This implementation uses a DP over the sum 3*n
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t dp[30001];       /* current DP values */
static seq_t g[30001];        /* temporary table for unlimited copies */
static seq_t newdp[30001];    /* new DP after processing a part size */
static bool initialized = false;   /* flag to build the tables only once */

seq_t a(idx_t n)
{
    /* Build DP table once for all sums up to 30000 (=3*10000). */
    if (!initialized) {
        const size_t MAX_SUM = 30000;      /* maximum 3*n we need to support */

        /* Base case: only the empty partition for sum 0. */
        for (size_t i = 0; i <= MAX_SUM; ++i)
            dp[i] = 0;
        dp[0] = 1;

        /* Process each power of 3 as a part size. */
        for (size_t w = 1; w <= MAX_SUM; w *= 3) {
            /* g[t] = sum_{m>=0} dp[t - m*w]  (unrestricted copies). */
            for (size_t t = 0; t <= MAX_SUM; ++t)
                g[t] = dp[t];
            for (size_t t = w; t <= MAX_SUM; ++t)
                g[t] += g[t - w];

            /* For each sum t, subtract the case where we use exactly one copy,
               because each part used must appear at least twice. */
            for (size_t t = 0; t <= MAX_SUM; ++t) {
                seq_t val = g[t];
                if (t >= w)
                    val -= dp[t - w];
                newdp[t] = val;
            }

            /* Move newdp into dp for next iteration. */
            for (size_t t = 0; t <= MAX_SUM; ++t)
                dp[t] = newdp[t];
        }

        initialized = true;
    }

    /* The required value is the coefficient of x^(3*n). */
    size_t idx = (size_t)((idx_t)n * (idx_t)3);
    return dp[idx];
}
