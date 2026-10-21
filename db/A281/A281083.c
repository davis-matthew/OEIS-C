/*
 * A281083  Expansion of Product_{k>=0} (1 + x^(5*k*(k+1)/2+1)).
 * https://oeis.org/A281083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct centered
 *            pentagonal numbers (generating function ∏_{k≥0}
 *            (1+x^{5*k*(k+1)/2+1}) )
 * Range:     u8 n <= 1786, u16 n <= 6390, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[n + 1];
#else
    dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!dp)
        return 0; /* allocation failed */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 0;; ++k)
    {
        idx_t part = 5 * k * (k + 1) / 2 + 1; /* centered pentagonal number */
        if (part > n)
            break;
        for (idx_t i = n; i >= part; --i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
