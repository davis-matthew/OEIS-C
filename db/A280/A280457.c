/*
 * A280457  Expansion of Product_{k>=0} (1 + x^(7*k+1)).
 * https://oeis.org/A280457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts congruent
 *            to 1 mod 7
 * Range:     u8 n <= 235, u16 n <= 609, u32 n <= 1797, u64 n <= 5824,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* Internal static storage for dynamic programming table. */
static seq_t *dp = NULL;
static idx_t max_n = 0;

static void recompute(idx_t n)
{
    /* Allocate a new table of size n+1. */
#if defined(__cplusplus)
    /* C++: use new[] so that object constructors are invoked if need be. */
    seq_t *tmp = new seq_t[n + 1];
#else
    /* C: use calloc to zero the memory. */
    seq_t *tmp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
#endif
    if (tmp == NULL) {
        /* Allocation failed – abort program (no alternative). */
        abort();
    }

    /* If we already had a table, free it. */
    if (dp != NULL) {
#if defined(__cplusplus)
        delete[] dp;
#else
        free(dp);
#endif
    }

    dp = tmp;
    /* Initialize DP: dp[0] = 1, others already zero (calloc or new[] gives zero). */
    dp[0] = (seq_t)1;

    /* 0‑1 knapsack over allowed distinct parts 1,8,15,… up to n. */
    for (idx_t w = (idx_t)1; w <= n; w += 7) {
        for (idx_t j = n; j >= w; --j) {
            dp[j] += dp[j - w];
        }
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 and DP must exist for n>0. */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    if (dp == NULL || n > max_n) {
        recompute(n);
    }

    return dp[n];
}
