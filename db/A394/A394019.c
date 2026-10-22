/*
 * A394019  Expansion of g.f.: Product_{k>=1} 1 / (1 - x^(k^2 + 4)).
 * https://oeis.org/A394019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 383, u32 n <= 1378, u64 n <= 6246,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming: partitions of n into parts of the form k^2 + 4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static size_t dp_len = 0;

    /* Ensure the DP array can hold indices 0 .. n */
    if ((size_t)n + 1 > dp_len) {
        seq_t *tmp = (seq_t *)realloc(dp, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp && n + 1 > 0) {
            free(dp);
            return 0;           /* allocation failure */
        }
        dp = tmp;
        dp_len = (size_t)n + 1;
    }

    /* Initialise DP array */
    dp[0] = 1;                         /* one way to form 0 */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* For each admissible part value v = k^2 + 4 (k >= 1) up to n */
    for (idx_t k = 1; ; ++k) {
        seq_t v_seq = (seq_t)k * (seq_t)k + 4;   /* part value as seq_t */
        if (v_seq > (seq_t)n) break;            /* no larger parts needed */
        idx_t v = (idx_t)v_seq;                  /* safe: v_seq <= n */

        for (idx_t i = v; i <= n; ++i)
            dp[i] += dp[i - v];
    }

    return dp[n];
}
