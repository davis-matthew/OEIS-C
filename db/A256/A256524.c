/*
 * A256524  Number of partitions of 3n into at most 4 parts.
 * https://oeis.org/A256524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 3n into at most 4 parts
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equal to the number of partitions of 3n with parts not exceeding 4, */
/* i.e. the number of solutions to a+2b+3c+4d = 3n in non‑negative integers. */
/* The implementation uses a straightforward DP over the four allowed part sizes. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Compute m = 3n using seq_t to avoid overflow of seq_t */
    seq_t m_seq = (seq_t)n * (seq_t)3;
    /* m_seq fits into idx_t because n will be reasonable; cast safely */
    idx_t m = (idx_t)m_seq;

    /* Special case: n = 0 */
    if (m == 0) return (seq_t)1;

    /* Static cache for DP array */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;

    /* Allocate / reallocate when needed */
    if (dp_cap <= m) {
        size_t new_cap = m + 1;
        seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;            /* Out of memory: return 0 as sentinel */
        if (dp) free(dp);
        dp = new_dp;
        dp_cap = new_cap;
    }

    /* Initialize DP array */
    dp[0] = (seq_t)1;
    for (idx_t j = 1; j <= m; ++j) dp[j] = (seq_t)0;

    /* Compute number of partitions with parts up to 4 */
    for (idx_t coin = 1; coin <= 4; ++coin) {
        for (idx_t j = coin; j <= m; ++j) {
            dp[j] += dp[j - coin];
        }
    }

    return dp[m];
}
