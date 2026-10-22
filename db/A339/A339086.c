/*
 * A339086  Number of compositions (ordered partitions) of n into distinct
 *          parts congruent to 1 mod 5.
 * https://oeis.org/A339086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into distinct parts congruent
 *            to 1 (mod 5). This implementation evaluates the generating
 *            function ∑_{k≥0} k! * x^{k(5k−3)/2} / ∏_{j=1..k} (1−x^{5j}) by
 *            enumerating k and counting partitions of n–base(k) with parts
 *            5,10,…,5k
 * Range:     u8 n <= 68, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t k;
    seq_t ans  = 0;
    seq_t fact = 1;           /* factorial for current k */

    static seq_t *dp = NULL;  /* shared DP buffer */
    static size_t dp_capacity = 0;

    for (k = 0; ; ++k) {
        /* base(k) = k*(5k−3)/2  (int safe for k up to ~63 when n≤10 000) */
        idx_t base = k * ((idx_t)5 * k - 3) / 2;
        if (base > n) {
            break;
        }

        if (k == 0) {                 /* only the empty composition for n==0 */
            if (n == 0) {
                ans += 1;              /* 0! * 1 */
            }
        } else {
            idx_t maxT = n - base;     /* size of the DP array needed */

            if (dp_capacity < (size_t)(maxT + 1)) {
                free(dp);
                dp = (seq_t *)malloc((maxT + 1) * sizeof(seq_t));
                dp_capacity = maxT + 1;
            }

            /* initialise dp[0] = 1, others 0 */
            for (size_t i = 0; i <= (size_t)maxT; ++i)
                dp[i] = 0;
            dp[0] = 1;

            /* unbounded knapsack with parts 5,10,…,5k */
            for (idx_t j = 1; j <= k; ++j) {
                idx_t w = 5 * j;
                for (idx_t t = w; t <= maxT; ++t)
                    dp[t] += dp[t - w];
            }

            ans += fact * dp[maxT];
        }

        /* update factorial for next k: (k+1)! = k! * (k+1) */
        fact *= (seq_t)(k + 1);
    }
    return ans;
}
