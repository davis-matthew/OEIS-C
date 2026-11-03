/*
 * A279329  Number of partitions of n into distinct cubes.
 * https://oeis.org/A279329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   Number of partitions of n into distinct cubes, computed by
 *            dynamic programming on the generating function ∏_{k≥1} (1 +
 *            x^{k^3})
 * Range:     u8 n <= 19691, u16 n <= 93716, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 103 terms through n = 9882 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

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
