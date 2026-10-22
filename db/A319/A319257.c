/*
 * A319257  Expansion of Product_{k>0} 1/(1 - k*(k+1)/2 * x^(k*(k+1)/2)).
 * https://oeis.org/A319257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k>0} 1/(1 - k*(k+1)/2 * x^{k*(k+1)/2})
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array of size n+1 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;            /* allocation failure */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* process triangular numbers T_k = k*(k+1)/2 */
    for (idx_t k = 1; ; ++k) {
        seq_t kk  = (seq_t)k;
        seq_t T   = kk * (kk + 1) / 2;   /* triangular number */
        idx_t Tidx = (idx_t)T;
        if (Tidx > n) break;            /* beyond target */

        seq_t weight = T;                /* factor per occurrence */

        /* unbounded knapsack: combine parts of size Tidx */
        for (idx_t s = Tidx; s <= n; ++s) {
            dp[s] += weight * dp[s - Tidx];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
