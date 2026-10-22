/*
 * A328325  Expansion of Product_{k>=0} 1/(1 - x^(k^k)).
 * https://oeis.org/A328325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} 1/(1 - x^(k^k))
 * Range:     u8 n <= 39, u16 n <= 330, u32 n <= 6991, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) return 0; /* allocation failure */

    /* Initialize DP: dp[0] = 1, others 0 */
    for (size_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process each part size r = k^k (0^0 treated as 1) */
    for (idx_t k = 0; ; ++k)
    {
        idx_t r;
        if (k == 0)
        {
            r = 1; /* 0^0 defined as 1 */
        }
        else
        {
            r = 1;
            for (idx_t i = 1; i <= k; ++i)
            {
                /* Prevent overflow and skip parts larger than n */
                if (r > n / k) { r = n + 1; break; }
                r *= k;
            }
        }
        if (r > n) break; /* no further parts needed */

        /* Update DP for unlimited copies of this part size */
        for (idx_t i = r; i <= n; ++i)
        {
            dp[i] += dp[i - r];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
