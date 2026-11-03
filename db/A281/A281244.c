/*
 * A281244  Expansion of Product_{k>=1} (1 + x^(6*k-1)).
 * https://oeis.org/A281244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 222, u16 n <= 555, u32 n <= 1588, u64 n <= 5070,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9660 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281244: number of subsets of {5,11,17,23,…} whose elements sum to n. */
/* a(0) = 1, a(5) = 1, a(11) = 1, a(16) = 1, a(17) = 1, …         */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of already computed values up to the greatest n seen */
    static seq_t *dp   = NULL;   /* dynamic array of a[0..dp_max]          */
    static idx_t  dp_max = 0;    /* highest index that has been computed   */
    static size_t dp_cap = 0;    /* allocated capacity of dp               */

    /* ---------- first call or new maximum ---------- */
    if (dp == NULL) {
        dp_cap = (size_t)n + 1;
        dp = mem_new(seq_t, dp_cap);
        if (!dp) return 0;          /* allocation failed, return 0           */
        for (size_t i = 0; i < dp_cap; ++i) dp[i] = 0;
        dp[0] = 1;                  /* empty sum                           */
        dp_max = n;

        /* compute the whole table up to n      */
        for (idx_t w = 5; w <= n; w += 6) {   /* each usable part 6k−1  */
            for (idx_t i = n; i >= w; --i) {
                dp[i] = dp[i] + dp[i - w];
            }
        }
        return dp[n];
    }

    /* ---------- cached value available ---------- */
    if (n <= dp_max)
        return dp[n];

    /* ---------- new maximum > current cache ---------- */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > dp_cap) {
        dp = mem_resize(seq_t, dp, new_cap);
        if (!dp) return 0;              /* allocation failed, return 0       */
        dp_cap = new_cap;
    }

    /* reset values                                 */
    for (size_t i = 0; i < dp_cap; ++i) dp[i] = 0;
    dp[0] = 1;
    dp_max = n;

    /* recompute the full table for the new maximum   */
    for (idx_t w = 5; w <= n; w += 6) {
        for (idx_t i = n; i >= w; --i) {
            dp[i] = dp[i] + dp[i - w];
        }
    }
    return dp[n];
}
