/*
 * A356737  Number of integer partitions of n into odd parts covering an
 *          interval of odd numbers.
 * https://oeis.org/A356737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd parts such that the
 *            set of distinct part sizes forms a contiguous interval of odd
 *            numbers. (Implementation via dynamic programming.)
 * Range:     u8 n <= 46, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allocate array for the final counts */
    seq_t *total;
#ifdef __cplusplus
    total = new seq_t[n + 1]();      /* zero‑initialized */
#else
    total = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif

    if (!total)
        return (seq_t)0; /* allocation failure */
    total[0] = 1;         /* a(0) = 1 */

    /* Temporary array for the coin‑change DP */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1]();
#else
    dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif

    if (!dp) {
#ifdef __cplusplus
        delete[] total;
#else
        free(total);
#endif
        return (seq_t)0;
    }

    /* Main computation */
    for (idx_t a_start = 1; a_start <= n; a_start += 2) {
        /* Reset dp for a new interval */
#ifdef __cplusplus
        std::fill(dp, dp + n + 1, seq_t(0));
#else
        memset(dp, 0, (n + 1) * sizeof(seq_t));
#endif
        dp[0] = 1;  /* one way to make 0 */

        idx_t sumS = 0;   /* sum of mandatory parts of the interval */
        for (idx_t w = a_start; sumS + w <= n; w += 2) {
            sumS += w;     /* add one mandatory part of size w */

            /* Unbounded coin change update with new coin w */
            for (idx_t s = w; s <= n - sumS; ++s) {
                dp[s] += dp[s - w];
            }

            /* Add contributions of this interval to the final totals */
            idx_t limit = n - sumS;
            for (idx_t r = 0; r <= limit; ++r) {
                total[sumS + r] += dp[r];
            }
        }
    }

    seq_t result = total[n];

#ifdef __cplusplus
    delete[] dp;
    delete[] total;
#else
    free(dp);
    free(total);
#endif

    return result;
}
