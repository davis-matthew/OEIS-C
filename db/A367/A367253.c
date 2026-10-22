/*
 * A367253  The number of ways of making change for 5n cents with Canadian
 *          coins (5, 10, 25, 100, 200).
 * https://oeis.org/A367253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n with parts 1, 2, 5, 20, 40
 * Range:     u8 n <= 52, u16 n <= 301, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Same as the number of ways to make change for 5n cents with Canadian coins) */

#include <stdlib.h>   /* calloc, free, abort */

static seq_t *dp_cache = NULL;
static idx_t cache_max = 0;
static const idx_t parts[5] = { 1, 2, 5, 20, 40 };

/* Return the number of partitions of n using the given parts. */
seq_t a(idx_t n) {
    if (dp_cache == NULL || n > cache_max) {
        /* Allocate new table up to n */
        idx_t new_max = n;
        seq_t *new_dp = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
        if (!new_dp) abort();          /* allocation failed */

        new_dp[0] = 1;                  /* one way to partition 0 */

        /* Classic coin-change DP: for each part, accumulate contributions. */
        for (int k = 0; k < 5; ++k) {
            idx_t p = parts[k];
            for (idx_t i = p; i <= new_max; ++i) {
                new_dp[i] += new_dp[i - p];
            }
        }

        /* Replace old cache */
        if (dp_cache) free(dp_cache);
        dp_cache = new_dp;
        cache_max = new_max;
    }

    return dp_cache[n];
}
