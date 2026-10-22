/*
 * A363263  Number of integer partitions of n covering an initial interval of
 *          positive integers with a unique co-mode.
 * https://oeis.org/A363263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of integer partitions of n covering an initial interval
 *            of positive integers with a unique co-mode
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t ans = 0;
    if (n == 0) return 0;

    /* maximum k with k(k+1)/2 <= n */
    idx_t k_max = 0;
    while (((k_max + 1) * (k_max + 2)) / 2 <= n) {
        ++k_max;
    }

    /* one DP buffer reused for every (k,t) pair */
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
#else
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif

    for (idx_t k = 1; k <= k_max; ++k) {
        long long S_ll = (long long)k * (k + 1) / 2;          /* sum 1..k */

        for (idx_t t = 1; t <= k; ++t) {
            long long max_m_ll = ((long long)n + t) / S_ll - 1;
            if (max_m_ll < 1) continue;

            long long max_R_ll = (long long)n - 2 * S_ll + t;
            if (max_R_ll < 0) continue;
            idx_t max_R = (idx_t)max_R_ll;

            /* initialize DP up to max_R */
            for (idx_t i = 0; i <= max_R; ++i) dp[i] = 0;
            dp[0] = 1;

            /* coin‑change DP using parts 1..k, skipping part t */
            for (idx_t i = 1; i <= k; ++i) {
                if (i == t) continue;
                for (idx_t s = i; s <= max_R; ++s) {
                    dp[s] += dp[s - i];
                }
            }

            /* accumulate contributions for each admissible m */
            for (long long mm = 1; mm <= max_m_ll; ++mm) {
                long long R_ll = (long long)n - (mm + 1) * S_ll + t;
                if (R_ll < 0) break;
                if (R_ll > (long long)max_R) continue;  /* safety guard */
                ans += dp[(idx_t)R_ll];
            }
        }
    }

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return ans;
}
