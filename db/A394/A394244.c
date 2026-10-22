/*
 * A394244  Expansion of Product_{k>=1} 1/(1 - x^(2*k^2 + 1)).
 * https://oeis.org/A394244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 2*k^2+1 (k>=1)
 * Range:     u8 n <= 182, u16 n <= 582, u32 n <= 2301, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple DP that builds the partition count
   for all integers up to n using the allowed part sizes. */

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Compute a(n) for the OEIS sequence A394244 */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0u)
        return 1;

    /* Allocate an array of seq_t for DP */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    dp[0] = 1;                     /* base case: one way to partition 0 */

    /* Generate allowed part sizes 2*k^2+1 (k >= 1) and update DP */
    for (idx_t k = 1;; ++k)
    {
        /* Compute part size safely to avoid overflow */
        idx_t part = (idx_t)(((__int128)k * k * 2) + 1);
        if (part > n)
            break;                 /* no larger parts will fit */

        /* Standard unbounded knapsack update */
        for (idx_t i = part; i <= n; ++i)
        {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
