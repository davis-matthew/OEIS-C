/*
 * A330242  Sum of largest emergent parts of the partitions of n.
 * https://oeis.org/A330242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * P(n) - Σ_{i=2}^{n-1} p_{≤i}(n) where P(n) is the
 *            number of partitions of n with parts in [2, n-1], and p_{≤i}(n)
 *            counts partitions of n into parts in [2, i]
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

/* Computes A330242 (sum of largest parts of partitions of n without 1 or n). */
seq_t a(idx_t n)
{
    /* For n < 1 the sum is zero. */
    if (n <= 1) return 0;

    /* Dynamic array for partition counts. Re-use across calls. */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;

    size_t new_cap = (size_t)(n + 1);
    if (dp_cap < new_cap) {
        seq_t *tmp = seq_realloc(dp, dp_cap, new_cap);
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Initialize DP array: dp[s] will hold number of partitions of s using parts ≤ current coin. */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t sum_p_le = 0;          /* Σ_{i=2}^{n-1} p_{≤i}(n) */
    seq_t partitions = 0;        /* P(n) at the end */

    /* Iterate over available part sizes from 2 up to n-1. */
    for (idx_t coin = 2; coin <= n - 1; ++coin) {
        for (idx_t s = coin; s <= n; ++s) {
            dp[s] = dp[s] + dp[s - coin];
        }
        sum_p_le = sum_p_le + dp[n];
    }

    partitions = dp[n];
    seq_t result = ((seq_t)n * partitions) - sum_p_le;
    return result;
}
