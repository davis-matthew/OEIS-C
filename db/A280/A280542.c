/*
 * A280542  Expansion of 1/(1 - Sum_{k>=2} x^(k^2)).
 * https://oeis.org/A280542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0; otherwise a(n) = Σ_{k≥2, k^2≤n} a(n−k^2)
 * Range:     u8 n <= 53, u16 n <= 94, u32 n <= 173, u64 n <= 331,
 *            u128 n <= 648, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280542: number of compositions of n into squares > 1. */
#include <stdlib.h>

static seq_t *dp = NULL;          /* DP table for a(n) */
static idx_t dp_len = 0;          /* Current length of dp (number of entries) */

seq_t a(idx_t n)
{
    /* Allocate or grow the DP table as necessary.  The table always keeps
     * a(0) through a(dp_len-1).  The table is never shrunk. */
    if (dp == NULL) {
        dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!dp) return 0;          /* allocation failed – return 0 */
        dp[0] = 1;                  /* a(0) = 1 */
        dp_len = 1;
    } else if (dp_len <= n) {
        seq_t *tmp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* realloc failed – return 0 */
        dp = tmp;
        /* dp_len remains the old length; we will fill new entries below. */
    }

    /* Compute new entries from dp_len up to n. */
    for (idx_t i = dp_len; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 2; k * k <= i; ++k) {
            sum += dp[i - k * k];
        }
        dp[i] = sum;
    }

    dp_len = n + 1;
    return dp[n];
}
