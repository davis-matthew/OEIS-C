/*
 * A308989  Sum of all the parts in the partitions of n into 8 parts.
 * https://oeis.org/A308989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * A026814(n) (A026814(n) = number of partitions of n
 *            into 8 parts)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 166, u64 n <= 2788,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9613 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Quick return for n < 8; p_8(n) = 0 then a(n) = 0 */
    if (n < 8) {
        return (seq_t)0;
    }

    /* Use dynamic arrays: dp[k][s] for 0 <= k <= 8, 0 <= s <= n */
    idx_t size = n + 1;
    seq_t **dp = (seq_t**)malloc(9 * sizeof(seq_t*));
    if (!dp) abort();

    for (int k = 0; k < 9; ++k) {
        dp[k] = mem_new(seq_t, size);
        if (!dp[k]) abort();
    }

    /* Base: one way to partition 0 into 0 parts */
    dp[0][0] = (seq_t)1;

    /* Compute p_k(s) for k = 1..8 */
    for (int k = 1; k < 9; ++k) {
        for (idx_t s = 0; s < size; ++s) {
            if (s < (idx_t)k) {
                dp[k][s] = (seq_t)0;
                continue;
            }
            seq_t part1 = dp[k][s - k];      /* partitions of s- k into k parts */
            seq_t part2 = dp[k - 1][s - 1];   /* partitions of s-1 into k-1 parts */
            dp[k][s] = part1 + part2;
        }
    }

    seq_t result = (seq_t)n * dp[8][n];

    /* Clean up allocated memory */
    for (int k = 0; k < 9; ++k) {
        mem_free(dp[k]);
    }
    free(dp);

    return result;
}
