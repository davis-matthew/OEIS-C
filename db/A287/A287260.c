/*
 * A287260  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks are <= ten.
 * https://oeis.org/A287260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of set partitions of [n] with every difference
 *            between consecutive block minima ≤10
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* handle offset 0 */
    if (n == 0) return (seq_t)1;

    /* capacity for dp arrays: (n+2) possible block counts × 11 distances */
    size_t cap = (size_t)(n + 2) * 11;
    seq_t *dp  = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_t *ndp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!dp || !ndp) {
        free(dp); free(ndp);
        return (seq_t)0;       /* allocation failure – unlikely in tests */
    }
    memset(dp, 0, cap * sizeof(seq_t));

    /* initial state: one block (minimum 0), distance 0 */
    dp[1 * 11 + 0] = (seq_t)1;
    idx_t max_k = 1;                /* maximum block count seen so far */

    /* process elements 1 .. n-1 */
    for (idx_t pos = 1; pos < n; ++pos) {
        /* clear next array */
        memset(ndp, 0, cap * sizeof(seq_t));

        for (idx_t k = 1; k <= max_k; ++k) {
            for (idx_t d = 0; d <= 10; ++d) {
                seq_t val = dp[k * 11 + d];
                if (!val) continue;

                /* assign current element to an existing block */
                idx_t newd = (d + 1 <= 10) ? d + 1 : 10;
                ndp[k * 11 + newd] += val * (seq_t)k;

                /* start a new block if allowed by the gap constraint */
                if (d + 1 <= 10) {
                    ndp[(k + 1) * 11 + 0] += val;
                }
            }
        }

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;

        if (max_k < n) max_k++;     /* at most one more block can appear */
    }

    /* sum all states – no elements remain after the last min */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= max_k; ++k) {
        for (idx_t d = 0; d <= 10; ++d) {
            result += dp[k * 11 + d];
        }
    }

    free(dp);
    free(ndp);
    return result;
}
