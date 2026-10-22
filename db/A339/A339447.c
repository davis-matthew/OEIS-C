/*
 * A339447  G.f.: Sum_{k>=0} x^(2^k) / Product_{j=1..2^k} (1 - x^j).
 * https://oeis.org/A339447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A339447: number of partitions of n whose largest part is a power of 2.
   Computed by dynamic programming: for each part size m we update the
   partition counts dp[i] (partitions of i using parts up to m).  When m is
   a power of two, the number of partitions of n having largest part m
   equals dp[n‑m] after the update of m, so we add dp[n‑m] to res[n].
   After all parts up to n have been processed, res[n] holds the desired term. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    size_t N = (size_t)n;                 /* array bounds are size_t */

    /* dp[i] = number of partitions of i using parts up to the current m */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    /* res[i] = desired count for i (partitions whose largest part is a power of 2) */
    seq_t *res = (seq_t *)calloc(N + 1, sizeof(seq_t));

    if (!dp || !res) {                    /* allocation failure */
        free(dp); free(res);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;                     /* one way to partition 0 */

    for (idx_t m = (idx_t)1; m <= n; ++m) {
        size_t ms = (size_t)m;            /* m cast to size_t for array indices */

        /* Update dp to include part size m */
        for (size_t i = ms; i <= N; ++i) {
            dp[i] += dp[i - ms];
        }

        /* If m is a power of two, accumulate contributions to res */
        if ((m & (m - (idx_t)1)) == (idx_t)0) {
            for (size_t i = ms; i <= N; ++i) {
                res[i] += dp[i - ms];
            }
        }
    }

    seq_t answer = res[N];

    free(dp);
    free(res);

    return answer;
}
