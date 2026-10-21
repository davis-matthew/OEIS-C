/*
 * A265833  Expansion of Product_{k>=1} 1/(1 - (5*k-3)*x^(5*k-3)).
 * https://oeis.org/A265833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 - (5k-3)x^{5k-3})
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 61, u64 n <= 125,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n > 0, a(n) = Σ_{m=5k-3≤n} m * a(n - m) */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate array for 0 … n */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                       /* allocation failure */

    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;                                /* empty partition */

    /* Iterate over all allowed part sizes m = 5k-3 */
    for (idx_t k = 1;; ++k) {
        idx_t m = 5 * k - 3;
        if (m > n) break;
        for (idx_t i = m; i <= n; ++i) {
            dp[i] = dp[i] + (seq_t)m * dp[i - m];
        }
    }

    seq_t ans = dp[n];
    free(dp);
    return ans;
}
