/*
 * A333147  Number of compositions of n that are either strictly increasing or
 *          strictly decreasing.
 * https://oeis.org/A333147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000009(n) – 1, where A000009(n) counts partitions of n
 *            into distinct parts. The computation uses a 1‑dimensional
 *            dynamic programming array that is re‑used across calls
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the DP array – reused between calls */
    static seq_t *dp  = NULL;   /* dp[i] = A000009(i) */
    static idx_t  cap = 0;      /* allocated size of dp */

    /* make sure we have room for indices 0..n */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (dp == NULL) abort();   /* allocation error */
        cap = new_cap;
    }

    /* initialise dp[0] = 1, dp[1..n] = 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* classic 1‑D DP for partitions into distinct parts */
    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t j = n; j >= part; --j) {
            dp[j] += dp[j - part];
        }
    }

    /* a(n) = 2*A000009(n) – 1 */
    seq_t result = dp[n];
    result += result;          /* 2 * dp[n] */
    result -= (seq_t)1;
    return result;
}
