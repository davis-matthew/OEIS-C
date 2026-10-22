/*
 * A337765  Number of addition triangles with apex n where all rows are weakly
 *          increasing.
 * https://oeis.org/A337765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of addition triangles with apex n where all rows
 *            are weakly increasing. Computed by iterating over all possible
 *            base lengths m, solving the weighted sum equation
 *            b0·2^(m-1)+∑_{k=1}^{m-1} c_k·S_k = n with b0≥1, c_k≥0. A DP over
 *            the “coins” S_k counts the solutions
 * Range:     u8 n <= 48, u16 n <= 298, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */

    /* Allocate DP array once; reused for each base length m */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;

    seq_t total = 0;

    /* Iterate over possible base lengths m = 1, 2, ... */
    for (idx_t m = 1; ; ++m)
    {
        idx_t S0 = ((idx_t)1) << (m - 1);
        if (S0 > n) break;          /* no further m can contribute */

        /* Binomial coefficients w[i] = C(m-1,i) */
        idx_t *w = (idx_t *)malloc(m * sizeof(idx_t));
        if (!w) { free(dp); return 0; }
        w[0] = 1;
        for (idx_t i = 1; i < m; ++i)
        {
            unsigned __int128 tmp = (unsigned __int128)w[i - 1] * (m - i);
            tmp /= i;
            w[i] = (idx_t)tmp;
        }

        /* Suffix sums S[k] = Σ_{i=k}^{m-1} w[i] (k = 0 .. m-1) */
        idx_t *S = (idx_t *)malloc(m * sizeof(idx_t));
        if (!S) { free(dp); free(w); return 0; }
        S[m - 1] = w[m - 1];
        for (idx_t k = m - 1; k > 0; --k)
            S[k - 1] = S[k] + w[k - 1];

        /* DP for coin change with coins S[1..m-1] (unlimited use) */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;

        for (idx_t k = 1; k < m; ++k)
        {
            idx_t coin = S[k];
            if (coin == 0 || coin > n) continue;
            for (idx_t sum = coin; sum <= n; ++sum)
                dp[sum] += dp[sum - coin];
        }

        /* Add contributions for each possible b0 ≥ 1 */
        for (idx_t b0 = 1; ; ++b0)
        {
            idx_t used = S0 * b0;
            if (used > n) break;
            idx_t rem = n - used;
            total += dp[rem];
        }

        free(w);
        free(S);
    }

    free(dp);
    return total;
}
