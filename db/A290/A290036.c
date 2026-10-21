/*
 * A290036  Number of set partitions of [n] having exactly seven blocks of
 *          size > 1.
 * https://oeis.org/A290036
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A290036
 *  Number of set partitions of [n] having exactly seven blocks of size > 1.
 *  Recurrence for k ≥ 1:
 *      a_k(n) = (k+1) * a_k(n-1) + (n-1) * a_{k-1}(n-2)
 *      a_0(n) = 1 for all n
 *      a_k(n) = 0 for n < 2k
 *  The desired sequence is a_7(n) with offset 14 (a(14) = 135135).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *dp[8] = {NULL};        /* dp[k][n] = a_k(n) for k=0..7 */
static idx_t last[8] = {0};           /* largest n for which dp[k] has been computed */
static size_t cap[8] = {0};           /* current capacity for each dp[k] */

/* Ensure that dp[k] has capacity up to n (inclusive) and compute new entries. */
static void ensure_dp(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    idx_t k;

    /* 1. Allocate or grow each dp array to required capacity. */
    for (k = 0; k <= 7; ++k)
    {
        size_t old_cap = cap[k];

        if (dp[k] == NULL)
        {
            dp[k] = seq_alloc(needed);
            if (dp[k] == NULL) abort();
            cap[k] = needed;
        }
        else if (old_cap < needed)
        {
            seq_t *tmp = seq_realloc(dp[k], old_cap, needed);
            if (tmp == NULL) abort();
            dp[k] = tmp;
            cap[k] = needed;
        }

        /* Initialise any newly added entries. */
        idx_t i;
        for (i = (idx_t)old_cap; i <= n; ++i)
        {
            if (k == 0)
                dp[0][i] = (seq_t)1;
            else
                dp[k][i] = (seq_t)0;
        }
    }

    /* 2. Compute recurrence for k = 1 .. 7. */
    for (k = 1; k <= 7; ++k)
    {
        idx_t start = last[k] + 1;
        if (start < (idx_t)(2 * k))
            start = (idx_t)(2 * k);

        for (idx_t m = start; m <= n; ++m)
        {
            seq_t term1 = (seq_t)(k + 1) * dp[k][m - 1];
            seq_t term2 = (seq_t)(m - 1) * dp[k - 1][m - 2];
            dp[k][m] = term1 + term2;
        }
        last[k] = n;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_dp(n);
    return dp[7][n];
}
