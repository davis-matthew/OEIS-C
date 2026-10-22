/*
 * A301334  a(n) = [x^n] 1/(1 + n*(1 - theta_2(sqrt(x))/(2*x^(1/8)))), where
 *          theta_2() is the Jacobi theta function.
 * https://oeis.org/A301334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in 1/(1 - n*Σ_{k≥1} x^{k(k+1)/2})
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute it by dynamic programming. For each fixed n we build a DP array
 * dp[0..n] where dp[k] is the coefficient of x^k in the generating function.
 * Recurrence: dp[0] = 1;
 *              dp[k] = n * Σ_{t triangular ≤ k} dp[k - t]
 * where triangular numbers t = m(m+1)/2, m ≥ 1.
 * The value a(n) is dp[n].
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate array for the DP values. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* fallback on allocation failure */

    dp[0] = (seq_t)1;                 /* a[0] = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = (seq_t)0;

        /* Enumerate triangular numbers t = m*(m+1)/2 ≤ k. */
        for (idx_t m = 1; ; ++m) {
            idx_t t = m * (m + 1) / 2;
            if (t > k) break;
            sum += dp[k - t];
        }

        dp[k] = (seq_t)n * sum;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
