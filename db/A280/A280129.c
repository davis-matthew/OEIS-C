/*
 * A280129  Expansion of Product_{k>=2} (1 + x^(k^2)).
 * https://oeis.org/A280129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct squares ≥4. dp[i] =
 *            number of ways to write i as sum of distinct squares 4,9,16,...
 *            Computation: dynamic programming with each square used at most
 *            once
 * Range:     u8 n <= 774, u16 n <= 2624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* Allocate dp[0 .. n] and initialise all entries to 0. */
    seq_t *dp = seq_alloc_new_array((size_t)n + 1);
    if (!dp)
        return 0;                 /* out‑of‑memory guard (should not happen) */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                     /* empty partition */

    /* Process each square k^2 (k ≥ 2) once, updating dp from high to low. */
    for (idx_t k = 2; k * k <= n; ++k) {
        idx_t square = k * k;
        for (idx_t i = n; i >= square; --i)
            dp[i] += dp[i - square];
    }

    seq_t result = dp[n];
    seq_free_array(dp);
    return result;
}
