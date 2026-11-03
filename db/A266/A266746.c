/*
 * A266746  G.f.: 1/((1-t^6)*(1-t)*(1-t^3)*(1-t^5)*(1-t^7)*(1-t^9)*(1-t^11)).
 * https://oeis.org/A266746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A266746: number of partitions of n into parts {1,3,5,6,7,9,11}. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

/* Return a(n), the number of partitions of n into the allowed parts. */
seq_t a(idx_t n)
{
    /* Allowed part sizes. */
    static const idx_t parts[] = {1, 3, 5, 6, 7, 9, 11};
    const size_t k = sizeof(parts) / sizeof(parts[0]);

    /* Allocate a DP array for all i from 0..n. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = alloc_seq(sz);
    if (!dp)                /* allocation failure: return 0 */
        return 0;

    /* Initialise to zero. */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;             /* empty partition */

    /* Classic unbounded‑knapsack (coin‑change) DP. */
    for (size_t pi = 0; pi < k; ++pi) {
        idx_t coin = parts[pi];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free_seq(dp);
    return result;
}
