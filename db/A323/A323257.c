/*
 * A323257  Number of partitions lambda of n that satisfy gcd(lambda_i, n-1) =
 *          1 for all i and for which the lattice simplex delta(lambda) is an
 *          antichain simplex.
 * https://oeis.org/A323257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with all parts coprime to (n-1)
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* malloc failed */

    /* Initialize DP */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    idx_t nminus1 = n - (idx_t)1;   /* n-1, safe for n>=1 */

    for (idx_t part = 1; part <= n; ++part)
    {
        /* Check gcd(part, n-1) == 1 */
        seq_t g = gcd((seq_t)part, (seq_t)nminus1);
        if (g == (seq_t)1)
        {
            size_t p = (size_t)part;
            for (size_t sum = p; sum <= N; ++sum)
            {
                dp[sum] += dp[sum - p];
            }
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
