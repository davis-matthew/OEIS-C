/*
 * A292415  a(n) = [x^n] Product_{k>=1} (1 / (1 - 2^n*x^k)).
 * https://oeis.org/A292415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{k≥1} 1/(1-2^n*x^k) (number of partitions of n
 *            weighted by 2^{n·#parts})
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;   /* allocation failed – return zero (unlikely in test) */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t mult = pow2(n);   /* mult = 2^n, the weight per part */

    for (idx_t k = 1; k <= n; ++k)
    {
        for (idx_t j = k; j <= n; ++j)
        {
            seq_t tmp = dp[j - k];
            tmp *= mult;          /* add contribution of a part of size k */
            dp[j] += tmp;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
