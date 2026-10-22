/*
 * A354736  a(0) = a(1) = 1; a(n) = 5 * Sum_{k=0..n-2} a(k) * a(n-k-2).
 * https://oeis.org/A354736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = 5 * Σ_{k=0}^{n-2} a(k) * a(n-2-k) for n
 *            ≥ 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;   /* cache of computed values */
    static idx_t  len  = 0;      /* number of elements stored in dp */

    /* extend the cache if necessary */
    if (len <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed – return 0 as safe fallback */
        dp = tmp;

        /* compute new entries dp[len] … dp[n] */
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                dp[i] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                idx_t end = i - 2;
                for (idx_t k = 0; k <= end; ++k) {
                    sum += dp[k] * dp[end - k];
                }
                dp[i] = 5 * sum;
            }
        }
        len = n + 1;
    }

    return dp[n];
}
