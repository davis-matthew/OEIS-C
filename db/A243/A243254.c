/*
 * A243254  Number of compositions of n into parts {3,4,5} when all parts 3,4
 *          and 5 are present.
 * https://oeis.org/A243254
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 44, u32 n <= 83, u64 n <= 162,
 *            u128 n <= 320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence-based DP:  a(n) = number of compositions of n using
 * parts {3,4,5} in which each part appears at least once.  The
 * DP stores, for every i ≤ n, the counts for each subset of parts
 * used in the composition.  Masks: bit0 = use 3, bit1 = use 4,
 * bit2 = use 5.  Dp[i][mask] gives the number of compositions of i
 * that use exactly the parts indicated by mask.  The desired count
 * is dp[n][7] (mask 111).
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of DP table */
static idx_t max_computed = 0;          /* largest n computed so far */
static seq_t *dp = NULL;                /* flattened table: dp[i*8 + mask] */

/* Grow DP table to at least size n+1 (i.e. entries 0..n). */
static void ensure_capacity(idx_t n)
{
    if (max_computed >= n)
        return;

    /* New table size */
    idx_t new_size = n + 1;
    /* Reallocate memory */
    dp = (seq_t *)realloc(dp, new_size * 8 * sizeof(seq_t));

    /* Initialize new entries to zero */
    for (idx_t i = max_computed + 1; i <= n; ++i)
        for (unsigned m = 0; m < 8; ++m)
            dp[i << 3 | m] = (seq_t)0;

    /* Base case: empty composition uses no parts */
    if (max_computed == 0)
        dp[0] = (seq_t)1;

    /* Fill DP for all new indices */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* Skip if all additions will be zero for this i (i < 3) */
        if (i < 3) continue;

        /* Contributions from part 3 (bit0) */
        if (i >= 3) {
            for (unsigned pm = 0; pm < 8; ++pm)
            {
                seq_t val = dp[(i - 3) << 3 | pm];
                if (val) dp[(i << 3) | (pm | 1)] += val;
            }
        }
        /* Contributions from part 4 (bit1) */
        if (i >= 4) {
            for (unsigned pm = 0; pm < 8; ++pm)
            {
                seq_t val = dp[(i - 4) << 3 | pm];
                if (val) dp[(i << 3) | (pm | 2)] += val;
            }
        }
        /* Contributions from part 5 (bit2) */
        if (i >= 5) {
            for (unsigned pm = 0; pm < 8; ++pm)
            {
                seq_t val = dp[(i - 5) << 3 | pm];
                if (val) dp[(i << 3) | (pm | 4)] += val;
            }
        }
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 12 (offset). */
    if (n < 12)
        return (seq_t)0;

    ensure_capacity(n);

    /* The desired count is dp[n][111] (mask 7). */
    return dp[n << 3 | 7];
}
