/*
 * A279759  Expansion of Product_{k>=1} 1/(1 - x^(k*(3*k-1)*(3*k-2)/2)).
 * https://oeis.org/A279759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into nonzero dodecahedral
 *            numbers (OEIS A279759)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[j] = number of ways to write j as a sum of dodecahedral numbers */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* allocation failure */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Generate dodecahedral numbers and perform coin‑change DP.
       Compute the part value using idx_t to avoid overflow in seq_t. */
    for (idx_t k = 1;; ++k) {
        idx_t kk = k;
        idx_t part = kk * (3 * kk - 1) * (3 * kk - 2) / 2;
        if (part > n) break;

        /* Standard partition DP: add this coin to all possible sums. */
        for (idx_t j = part; j <= n; ++j)
            dp[j] += dp[j - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
