/*
 * A285244  Expansion of Product_{k>=1} (1 + k*x^(k^2)).
 * https://oeis.org/A285244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{S⊆{1,2,…}, Σ_{k∈S}k² = n} ∏_{k∈S}k (Coefficient of
 *            xⁿ in ∏_{k≥1}(1 + k·x^{k²}) )
 * Range:     u8 n <= 73, u16 n <= 219, u32 n <= 744, u64 n <= 2939,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for coefficients up to n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out‑of‑memory guard */

    /* Initialize */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* 0‑1 knapsack DP over square indices */
    idx_t kmax = 0;
    for (idx_t k = 1; k * k <= n; ++k) kmax = k;   /* floor sqrt(n) */

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t sq = k * k;
        seq_t kv = (seq_t)k;
        for (idx_t i = n; i >= sq; --i) {
            dp[i] = dp[i] + dp[i - sq] * kv;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
