/*
 * A261771  Expansion of Product_{k>=1} (1 + x^k) / (1 + x^(8*k)).
 * https://oeis.org/A261771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts that are
 *            not multiples of 8. The function computes this using dynamic
 *            programming over the allowed part sizes
 * Range:     u8 n <= 31, u16 n <= 87, u32 n <= 267, u64 n <= 897,
 *            u128 n <= 3210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for dynamic programming */
    static seq_t *dp      = NULL;   /* dp[i] = a(i) for i <= max_n */
    static idx_t   max_n = 0;        /* highest index computed so far */

    /* Allocate and initialise cache for the very first call. */
    if (!dp)
    {
        dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!dp) return (seq_t)0;      /* allocation failure – unlikely */
        dp[0] = (seq_t)1;              /* empty partition */
        max_n = n;
        return dp[n];
    }

    /* If we need values beyond the current maximum, recompute the cache
       up to the new maximum.  This is quadratic in the new maximum but
       acceptable for n up to about 10 000. */
    if (n > max_n)
    {
        /* Reallocate to hold values up to the requested n. */
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * (size_t)(n + 1));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        dp = tmp;

        /* Reinitialise dp to zero and set dp[0] = 1. */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Fill the table by processing all allowed part sizes up to n. */
        for (idx_t part = 1; part <= n; ++part)
        {
            if (part % 8 == 0) continue;   /* skip parts that are multiples of 8 */
            for (idx_t sum = n; sum >= part; --sum)
                dp[sum] += dp[sum - part];
        }

        max_n = n;
        return dp[n];
    }

    /* Cached value is already available. */
    return dp[n];
}
