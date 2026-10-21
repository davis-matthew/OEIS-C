/*
 * A279329  Number of partitions of n into distinct cubes.
 * https://oeis.org/A279329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Number of partitions of n into distinct cubes, computed by
 *            dynamic programming on the generating function ∏_{k≥1} (1 +
 *            x^{k^3})
 * Range:     u8 n <= 9882, u16 n <= 9882, u32 n <= 9882, u64 n <= 9882,
 *            u128 n <= 9882, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* allocate dp[0..n]; dp[i] counts partitions of i */
    seq_t *dp = alloc_seq_array(n + 1);
    dp[0] = (seq_t)1;

    /* iterate over all cube sizes k^3 ≤ n */
    for (idx_t k = 1; ; ++k) {
        idx_t cube = k * k * k;
        if (cube > n) break;

        /* standard 0/1 knapsack update in reverse order
           to keep each cube distinct */
        for (idx_t i = n; i >= cube; --i) {
            dp[i] = dp[i] + dp[i - cube];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
