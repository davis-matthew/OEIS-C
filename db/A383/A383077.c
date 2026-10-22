/*
 * A383077  Number of n-digit positive integers where all pairs of consecutive
 *          digits have a difference of at least 5.
 * https://oeis.org/A383077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 9
   Let dp[n][d] = number of n‑digit positive integers ending with digit d (0‑9).
   dp[1][d] = (d > 0) ? 1 : 0
   dp[n][e] = sum_{d=0..9, |d-e| >= 5} dp[n-1][d]
   a(n) = sum_{e=0..9} dp[n][e]
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static cache for computed a(k) values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_max = 0;

    /* static DP arrays for current and previous row */
    static seq_t dp_prev[10];
    static seq_t dp_curr[10];

    /* Ensure cache capacity */
    if (n + 1 > cache_cap)
    {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first row (n = 1) if not done yet */
    if (computed_max == 0)
    {
        /* dp[1][d] */
        for (int d = 0; d < 10; ++d)
            dp_prev[d] = 0;
        for (int d = 1; d <= 9; ++d)
            dp_prev[d] = 1;

        /* a(1) = 9 */
        cache[1] = 9;
        computed_max = 1;
    }

    /* Compute values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        /* For each ending digit e */
        for (int e = 0; e < 10; ++e)
        {
            seq_t sum = 0;
            for (int d = 0; d < 10; ++d)
            {
                int diff = d - e;
                if (diff < 0)
                    diff = -diff;
                if (diff >= 5)
                    sum += dp_prev[d];
            }
            dp_curr[e] = sum;
        }

        /* Sum over all ending digits to get a(i) */
        seq_t total = 0;
        for (int e = 0; e < 10; ++e)
            total += dp_curr[e];
        cache[i] = total;

        /* Prepare for next iteration */
        for (int e = 0; e < 10; ++e)
            dp_prev[e] = dp_curr[e];
    }

    computed_max = n;
    return cache[n];
}
