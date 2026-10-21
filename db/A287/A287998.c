/*
 * A287998  Expansion of 1/((1-x)(1-x^3)(1-x^5) ... (1-x^15)).
 * https://oeis.org/A287998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 100, u32 n <= 608, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = number of partitions of n into odd parts ≤ 15 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static size_t cap = 0;

    /* Ensure storage for indices 0 … n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *p = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!p) return 0;               /* allocation failure */
        dp  = p;
        cap = new_cap;
    }

    /* Initialize DP array */
    dp[0] = 1;
    for (size_t i = 1; i <= (size_t)n; ++i) dp[i] = 0;

    /* Odd parts ≤ 15 */
    const int parts[8] = {1, 3, 5, 7, 9, 11, 13, 15};
    for (int pi = 0; pi < 8; ++pi) {
        idx_t part = (idx_t)parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[(size_t)i] += dp[(size_t)(i - part)];
        }
    }

    return dp[(size_t)n];
}
