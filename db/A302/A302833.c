/*
 * A302833  Expansion of (1/(1 - x))*Product_{k>=1} 1/(1 - x^(k^2)).
 * https://oeis.org/A302833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n} p_s(k), where p_s(k) is the number of
 *            partitions of k into squares. Equivalently, a(n) is the partial
 *            sums of A001156
 * Range:     u8 n <= 28, u16 n <= 122, u32 n <= 629, u64 n <= 3760,
 *            u128 n <= 9678, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array for partitions into squares. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* fallback in case of allocation failure */

    idx_t i, j;
    /* Initialize DP: p_s(0) = 1, others 0. */
    for (i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = (seq_t)1;

    /* Classic coin‑change DP: squares as coin denominations. */
    for (i = 1; i * i <= n; i++) {
        idx_t s = i * i;
        for (j = s; j <= n; j++) {
            dp[j] = dp[j] + dp[j - s];
        }
    }

    /* Compute the partial sum a(n). */
    seq_t sum = (seq_t)0;
    for (i = 0; i <= n; i++) {
        sum = sum + dp[i];
    }

    free(dp);
    return sum;
}
