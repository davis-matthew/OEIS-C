/*
 * A308998  Sum of the largest parts in the partitions of n into 8 parts.
 * https://oeis.org/A308998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A308998: Sum of the largest parts in the partitions of n into 8 parts
 *
 *  a(n) = sum_{p=1..⌊n/8⌋} sum_{o=p..⌊(n-p)/7⌋} sum_{m=o..⌊(n-o-p)/6⌋}
 *          sum_{l=m..⌊(n-m-o-p)/5⌋} sum_{k=l..⌊(n-l-m-o-p)/4⌋}
 *          sum_{j=k..⌊(n-k-l-m-o-p)/3⌋} sum_{i=j..⌊(n-j-k-l-m-o-p)/2⌋}
 *          (n-i-j-k-l-m-o-p).
 *
 *  This implementation uses dynamic programming.  For each possible
 *  largest part `m` the number of partitions of `n-m` into exactly
 *  7 parts with all parts ≤ m is accumulated by the classic coin‑change
 *  recurrence.  The sum of the largest parts is accumulated as
 *  `∑ m * count_7_parts(n-m, ≤ m)`  for `m` from 1 to `n-7`.  The
 *  routine works for any `n ≥ 0` and for all defined `seq_t` types;
 *  it uses only integer arithmetic and standard C library calls.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* partitions of any size are irrelevant when n < 8 */
    if (n < 8)
        return 0;

    /* Max sum that will be represented in the DP array:
       the remaining 7 parts sum up to at most n-1. */
    idx_t maxS = n - 1;
    size_t len = (size_t)maxS + 1;          /* array size */

    /* static storage reused between calls */
    static seq_t *dp[8] = {0};              /* dp[k][s] – k parts, sum s */
    static size_t cache_len = 0;

    /* Allocate or resize the DP arrays as needed. */
    if (len > cache_len) {
        for (int k = 0; k < 8; ++k) {
            if (dp[k])
                free(dp[k]);
            dp[k] = (seq_t *)malloc(len * sizeof(seq_t));
            if (!dp[k])
                abort();                    /* allocation failed */
        }
        cache_len = len;
    }

    /* Initialise to zero.  seq_t is an integer type, so memset is safe. */
    for (int k = 0; k < 8; ++k)
        memset(dp[k], 0, len * sizeof(seq_t));

    /* There is exactly one way to partition 0 into 0 parts. */
    dp[0][0] = 1;

    seq_t sum = 0;

    /* For each possible largest part m (1 … n-7) */
    for (idx_t m = 1; m <= n - 7; ++m) {
        /* Update the DP tables with the new part size m */
        for (int parts = 1; parts <= 7; ++parts) {
            for (idx_t s = m; s <= maxS; ++s) {
                dp[parts][s] += dp[parts - 1][s - m];
            }
        }

        /* The remaining sum after choosing the largest part m. */
        idx_t rem = n - m;
        /* Count of partitions of rem into exactly 7 parts, all ≤ m. */
        seq_t ways = dp[7][rem];
        /* Accumulate contribution of this largest part. */
        sum += (seq_t)m * ways;
    }

    return sum;
}
