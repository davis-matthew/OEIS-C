/*
 * A338675  Expansion of Product_{k>=1} 1 / (1 - 6^(k-1)*x^k).
 * https://oeis.org/A338675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} p(n,k) * 6^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over integer partitions, where
 * dp[j] holds the sum of 6^(j-i) times the number of partitions of j
 * using parts up to i.  Each part size i contributes a factor
 * 6^(i-1), so the running weight w is multiplied by 6 each outer
 * iteration.  The algorithm is O(n^2) time and O(n) memory. */

#include <stdlib.h>  /* malloc, free */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for DP.  size_t is guaranteed to fit n+1 for n up to 10k. */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;  /* caller may handle allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* one way to partition 0 */

    seq_t w = (seq_t)1;   /* 6^(i-1) for current part size i */

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; ++j) {
            dp[j] = dp[j] + dp[j - i] * w;
        }
        w = w * (seq_t)6;  /* prepare weight for next part size i+1 */
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
