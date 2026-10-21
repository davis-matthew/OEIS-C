/*
 * A266748  G.f.:
 *          1/((1-t^8)*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^9)*(1-t^11)*(1-t^13)*(1-t^15)).
 * https://oeis.org/A266748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n using parts
 *            {1,3,5,7,8,9,11,13,15}
 * Range:     u8 n <= 27, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *dp = NULL;
    static size_t cache_size = 0;
    /* All allowed part sizes */
    static const idx_t parts[] = {1,3,5,7,8,9,11,13,15};
    static const size_t part_cnt = sizeof(parts)/sizeof(parts[0]);

    /* Expand cache and recompute if needed */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp)
            abort();                     /* memory allocation failed */

        /* initialise all entries to zero */
        for (size_t i = 0; i < new_size; ++i)
            new_dp[i] = 0;

        new_dp[0] = 1;                    /* one way to form 0 */

        /* dynamic‑programming: unbounded coin change */
        for (size_t p = 0; p < part_cnt; ++p)
        {
            size_t coin = (size_t)parts[p];
            for (size_t i = coin; i < new_size; ++i)
                new_dp[i] += new_dp[i - coin];
        }

        /* free old cache (if any) and replace */
        if (dp) free(dp);
        dp = new_dp;
        cache_size = new_size;
    }

    return dp[n];
}
