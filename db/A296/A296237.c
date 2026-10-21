/*
 * A296237  Expansion of Product_{k>0} 1/(1 - x^(k*(3*k+1)/2)).
 * https://oeis.org/A296237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 427, u32 n <= 1717, u64 n <= 8425,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296237: number of partitions of n into parts of the form
   k*(3*k+1)/2, for k >= 1.  This function computes the coefficient
   in the expansion of ∏_{k>0} 1/(1 - x^{k(3k+1)/2}).  */
/*   The DP below uses the standard coin-change recurrence:
      dp[0] = 1;  for each part m <= n, for j=m..n: dp[j] += dp[j-m].
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a work array of length n+1. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(len * sizeof(*dp));
    if (!dp) return 0;          /* allocation failed – return 0 */

    /* initialise dp[0..n] */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* DP over all possible part sizes m = k*(3k+1)/2 */
    for (idx_t k = 1; ; ++k)
    {
        idx_t m = k * (3 * k + 1) / 2;
        if (m > n) break;

        for (idx_t j = m; j <= n; ++j)
            dp[j] += dp[j - m];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
