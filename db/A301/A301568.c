/*
 * A301568  Expansion of Product_{k>=1} (1 + x^(5*k))*(1 + x^(5*k-3)).
 * https://oeis.org/A301568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts congruent
 *            to 0 or 2 mod 5
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and initialise DP array */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (dp == NULL) return 0;  /* graceful fallback on allocation failure */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Generate all allowed part sizes in ascending order */
    for (idx_t k = 1; ; ++k)
    {
        idx_t p2 = 5 * k - 3; /* part ≡ 2 (mod 5) */
        idx_t p0 = 5 * k;     /* part ≡ 0 (mod 5) */

        if (p2 > n && p0 > n) break;

        if (p2 <= n)
        {
            for (long long t = (long long)n; t >= (long long)p2; --t)
                dp[(size_t)t] += dp[(size_t)(t - p2)];
        }

        if (p0 <= n)
        {
            for (long long t = (long long)n; t >= (long long)p0; --t)
                dp[(size_t)t] += dp[(size_t)(t - p0)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
