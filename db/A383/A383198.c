/*
 * A383198  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 3.
 * https://oeis.org/A383198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of positive integers with n digits in which each
 *            pair of adjacent digits differs by at most 3 (digits 0–9, first
 *            digit ≠ 0). Computed by dynamic programming over the last digit
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache          = NULL;   /* a(1), a(2), … */
    static idx_t cache_size      = 0;      /* allocated size of cache    */
    static idx_t computed_upto   = 0;      /* highest n for which cache
                                            has been filled           */
    static seq_t dp[10]          = {0};     /* dp[ digit ] for current
                                            length                     */
    static int  initialized      = 0;      /* 0 → not yet initialised   */

    /* initialise on first entry */
    if (!initialized) {
        cache_size = 0;
        cache = NULL;
        initialized = 1;
    }

    if (n == 0) return 0;   /* offset is 1 – undefined for n=0 */

    /* extend cache if necessary */
    if (n > cache_size) {
        cache_size = n;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* initialise for length 1 if we have not yet */
    if (computed_upto == 0) {
        for (int d = 0; d < 10; ++d) dp[d] = 0;
        for (int d = 1; d <= 9; ++d) dp[d] = 1;

        seq_t sum = 0;
        for (int d = 0; d < 10; ++d) sum += dp[d];
        cache[0] = sum;          /* a(1) */
        computed_upto = 1;
    }

    /* compute up to requested n, extending cache and dp */
    seq_t tmp[10];
    for (idx_t len = computed_upto + 1; len <= n; ++len) {
        /* reset tmp */
        for (int d = 0; d < 10; ++d) tmp[d] = 0;

        /* transition from previous length */
        for (int d = 0; d < 10; ++d) {
            seq_t val = dp[d];
            if (val == 0) continue;
            for (int delta = -3; delta <= 3; ++delta) {
                int nd = d + delta;
                if (nd < 0 || nd > 9) continue;
                tmp[nd] += val;
            }
        }

        /* copy tmp back to dp for next iteration */
        for (int d = 0; d < 10; ++d) dp[d] = tmp[d];

        /* sum dp to obtain a(len) */
        seq_t sum = 0;
        for (int d = 0; d < 10; ++d) sum += dp[d];
        cache[len - 1] = sum;
    }

    computed_upto = n;
    return cache[n - 1];
}
