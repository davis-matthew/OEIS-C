/*
 * A301567  Expansion of Product_{k>=1} (1 + x^(5*k))*(1 + x^(5*k-4)).
 * https://oeis.org/A301567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A301567: number of partitions of n into distinct parts
   congruent to 0 or 1 (mod 5).  The function recomputes the value
   directly by the standard 0/1 knapsack DP for each call. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to avoid reallocating for every call */
    static seq_t *dp = NULL;      /* dp[0..max_n] */
    static size_t cap = 0;        /* current capacity (max_n+1) */

    /* allocate or grow the cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_dp = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!new_dp) return 0;   /* allocation failure */
        if (dp) free(dp);
        dp   = new_dp;
        cap  = new_cap;
    }

    /* reset dp[0..n] to 0, then set the base case */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* iterate over all allowed part values (distinct parts) */
    for (idx_t part = 1; part <= n; ++part) {
        if ((part % 5 == 0) || (part % 5 == 1)) {
            /* backwards loop to enforce distinctness */
            for (idx_t sum = n; sum >= part; --sum)
                dp[sum] += dp[sum - part];
        }
    }

    return dp[n];
}
