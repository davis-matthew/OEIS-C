/*
 * A293814  Number of partitions of n into distinct nontrivial divisors of n.
 * https://oeis.org/A293814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct nontrivial divisors of n
 * Range:     u8 n <= 179, u16 n <= 719, u32 n <= 2519, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 117 terms through n = 9551 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of subsets of divisors d | d|n, 1<d<n, with sum n        */

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

/* External linkage function as required.  */
seq_t a(idx_t n)
{
    /* Allocate DP array of length n+1 (dp[0..n]) */
    seq_t *dp = alloc_seq_array(n + 1);
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                     /* empty sum */

    /* Enumerate all nontrivial divisors of n and update DP */
    for (idx_t d = 2; d < n; ++d) {
        if (n % d == 0) {
            for (idx_t s = n; s >= d; --s) {
                dp[s] = dp[s] + dp[s - d];
            }
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
