/*
 * A346798  Number of partitions of n into parts congruent to 0, 3 or 4 (mod
 *          7).
 * https://oeis.org/A346798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 0, 3,
 *            or 4 modulo 7. Computed by a simple coin–change dynamic
 *            programming over all allowed part sizes up to n
 * Range:     u8 n <= 47, u16 n <= 114, u32 n <= 319, u64 n <= 1009,
 *            u128 n <= 3471, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP table for values 0..n */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;          /* allocation failed – unlikely */

    dp[0] = 1;                  /* one way to sum to 0 */

    /* Generate all part sizes k*7, k*7-3, k*7-4 that are ≤ n */
    idx_t maxk = (n + 6) / 7;   /* ceil(n/7) */
    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t parts[3] = { 7 * k, 7 * k - 3, 7 * k - 4 };
        for (int p = 0; p < 3; ++p)
        {
            idx_t coin = parts[p];
            if (coin > n) continue;

            for (idx_t i = coin; i <= n; ++i)
                dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
