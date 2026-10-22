/*
 * A328790  Expansion of (chi(x) / chi(-x^6))^2 in powers of x where chi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A328790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^(2k-1))^2 * (1 +
 *            x^(6k))^2
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static arrays to cache computed values up to the maximal n seen so far. */
    static seq_t *dp = NULL;          /* Current DP array */
    static seq_t *next_arr = NULL;    /* Temporary array for updates */
    static idx_t cur_max = 0;         /* Largest n that has been computed */

    /* First call – allocate the smallest possible arrays */
    if (dp == NULL) {
        dp = (seq_t*)calloc(1, sizeof(seq_t));
        if (!dp) return (seq_t)0;      /* OOM – unlikely in controlled tests */
        dp[0] = 1;
        next_arr = (seq_t*)calloc(1, sizeof(seq_t));
        cur_max = 0;
    }

    /* If a larger n is requested, recompute from scratch up to that n. */
    if (n > cur_max) {
        size_t new_size = n + 1;
        dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        next_arr = (seq_t*)realloc(next_arr, new_size * sizeof(seq_t));
        if (!dp || !next_arr) return (seq_t)0;  /* OOM */

        /* Clear the extended part of dp.  dp[0] must be 1. */
        dp[0] = 1;
        for (size_t i = 1; i <= n; ++i) dp[i] = 0;

        /* Main dynamic‑programming loop.
           For every admissible part m we multiply the current
           generating function by (1 + 2*x^m + x^(2m)). */
        for (idx_t m = 1; m <= n; ++m) {
            if (!((m & 1) || (m % 6 == 0))) continue;   /* m must be odd or a multiple of 6 */

            for (idx_t j = 0; j <= n; ++j) {
                seq_t val = dp[j];
                if (j >= m)       val += (seq_t)2 * dp[j - m];
                if (j >= 2 * m)   val += dp[j - 2 * m];
                next_arr[j] = val;
            }
            /* Prepare for the next part: swap dp and next_arr */
            seq_t *tmp = dp; dp = next_arr; next_arr = tmp;
        }
        cur_max = n;
    }

    return dp[n];
}
