/*
 * A294624  Number of partitions of n into distinct generalized octagonal
 *          numbers (A001082).
 * https://oeis.org/A294624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into distinct generalized octagonal
 *            numbers (OEIS A294624). Implements a dynamic programming
 *            approach that rebuilds the table only when a larger n is
 *            requested
 * Range:     u8 n <= 499, u16 n <= 1764, u32 n <= 7751, u64 n <= 9328,
 *            u128 n <= 9328, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp = NULL;          /* DP table: dp[s] = number of partitions of s */
static size_t dp_cap = 0;         /* allocated size of dp array */

/* Temporary array to hold all generalized octagonal numbers ≤ current n. */
static idx_t *items = NULL;
static size_t items_cnt = 0;

/* Rebuild the DP table for the largest n requested.  All values <= n are
   fully computed using the product ∏ (1 + x^s) over all distinct
   generalized octagonal numbers s ≤ n. */
static void rebuild(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Build the list of relevant generalized octagonal numbers ≤ n.       */
    /* ------------------------------------------------------------------ */
    if (items) {
        /* free previous memory if any */
        free(items);
    }
    items_cnt = 0;
    size_t cap = (size_t)(n) + 1;          /* big enough for all items */
    items = (idx_t *)malloc(cap * sizeof(idx_t));
    for (idx_t k = 1; ; ++k) {
        idx_t v1 = k * ((idx_t)3 * k - 2);   /* k*(3k‑2)   */
        idx_t v2 = k * ((idx_t)3 * k + 2);   /* k*(3k+2)   */
        if (v1 > n && v2 > n) break;
        if (v1 <= n) items[items_cnt++] = v1;
        if (v2 <= n) items[items_cnt++] = v2;
    }

    /* ------------------------------------------------------------------ */
    /* Allocate or reallocate the DP table for indices 0 … n.              */
    /* ------------------------------------------------------------------ */
    dp_cap = (size_t)n + 1;
    dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
    for (size_t i = 0; i < dp_cap; ++i) {
        dp[i] = 0;                          /* zero all entries            */
    }
    dp[0] = 1;                              /* empty partition             */

    /* ------------------------------------------------------------------ */
    /* 0/1 knapsack: process each item exactly once.                      */
    /* ------------------------------------------------------------------ */
    for (size_t ii = 0; ii < items_cnt; ++ii) {
        idx_t v = items[ii];
        for (idx_t sum = n; sum >= v; --sum) {
            dp[sum] = dp[sum] + dp[sum - v];
        }
    }
}

/* Main sequence function.  The table is rebuilt only when a larger n is
   requested. */
seq_t a(idx_t n)
{
    if (n >= dp_cap) {
        rebuild(n);
    }
    return dp[n];
}
