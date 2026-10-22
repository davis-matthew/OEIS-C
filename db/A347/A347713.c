/*
 * A347713  Number of compositions (ordered partitions) of n into at most 6
 *          squares.
 * https://oeis.org/A347713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into at most 6 squares (OEIS A347713).  */
/* The value equals the sum over k = 0..6 of the number of
   compositions of n into exactly k perfect‑square parts.  */
/* The implementation uses lazy dynamic programming with static
   memoisation tables.  */

#include <stdlib.h>

/* Global memoisation tables.
   dp[k][s] == number of compositions of s into exactly k squares. */
static seq_t **dp = NULL;
static size_t  dp_cap = 0;  /* current capacity of each dp[k] array */

/* Ensure tables are computed up to at least s = n. */
static void ensure_capacity(idx_t n)
{
    if (dp_cap > (size_t)n) return;

    size_t old_cap = dp_cap;
    size_t new_cap = (size_t)n + 1;  /* include index n */

    if (!dp) {
        dp = (seq_t**)malloc(7 * sizeof(seq_t*));
        for (int k = 0; k < 7; ++k) {
            dp[k] = (seq_t*)calloc(new_cap, sizeof(seq_t));
        }
        dp[0][0] = (seq_t)1;   /* exactly 0 squares sum to 0 */
    } else {
        for (int k = 0; k < 7; ++k) {
            dp[k] = (seq_t*)realloc(dp[k], new_cap * sizeof(seq_t));
            for (size_t i = old_cap; i < new_cap; ++i) dp[k][i] = (seq_t)0;
        }
    }

    /* Compute new entries. */
    for (size_t s = old_cap; s < new_cap; ++s) {
        for (int k = 1; k <= 6; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t r = 1; (idx_t)(r * r) <= (idx_t)s; ++r) {
                sum += dp[k-1][s - (size_t)(r * r)];
            }
            dp[k][s] = sum;
        }
    }

    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    seq_t res = (seq_t)0;
    for (int k = 0; k <= 6; ++k) {
        res += dp[k][(size_t)n];
    }
    return res;
}
