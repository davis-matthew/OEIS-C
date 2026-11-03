/*
 * A292547  Number of partitions of n into distinct odd cubes.
 * https://oeis.org/A292547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct odd cubes.
 *            Computed by dynamic programming over the odd cubes ≤ n
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 173 terms through n = 9177 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Allocate DP array for sums 0..n, initialize to zero. */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return (seq_t)0;  /* unlikely, fallback */
    dp[0] = 1;                /* empty partition */

    /* Iterate over odd cubes: (2*k-1)^3, stop when cube > n. */
    for (idx_t k = 1; ; ++k) {
        idx_t odd = 2 * k - 1;
        /* Use 128-bit arithmetic to avoid overflow when computing cube. */
        unsigned __int128 cube128 = (unsigned __int128)odd * odd * odd;
        if (cube128 > n) break;
        idx_t cube = (idx_t)cube128;

        /* Update DP in reverse to enforce distinctness. */
        for (idx_t sum = n; sum >= cube; --sum) {
            dp[sum] += dp[sum - cube];
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
