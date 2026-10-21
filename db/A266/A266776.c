/*
 * A266776  Molien series for invariants of finite Coxeter group A_7.
 * https://oeis.org/A266776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 94, u32 n <= 689, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A266776: number of partitions of n into parts 2,3,…,8.
   Uses dynamic programming with memoised array dp[0..n].
   Complexity O(7n).  Works for any seq_t that supports +,-,*,
   division and % operators. */
#include <stdlib.h>

extern seq_t a(idx_t n);      /* forward declaration for the compiler */

static seq_t *dp = NULL;      /* DP cache for already computed values   */
static size_t cache_size = 0; /* allocated size of dp (>= n+1)          */

static void resize_cache(size_t needed)
{
    if (needed <= cache_size)
        return;

    #ifdef __cplusplus
        /* Allocate a new array.  The old one must be deleted. */
        seq_t *new_dp = new seq_t[needed];
        if (dp) delete[] dp;
    #else
        /* In C, realloc handles freeing the old buffer. */
        seq_t *new_dp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        if (!new_dp)
            abort();                       /* allocation failure */
    #endif

    dp = new_dp;
    cache_size = needed;
}

seq_t a(idx_t n)
{
    /* Ensure cache large enough for indices 0..n. */
    resize_cache((size_t)n + 1);

    /* Initialise dp[0..n] for this call. */
    for (size_t i = 1; i <= (size_t)n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Dynamic programming: add each allowed part one by one. */
    static const int parts[] = {2, 3, 4, 5, 6, 7, 8};
    for (int p = 0; p < 7; ++p) {
        size_t coin = (size_t)parts[p];
        for (size_t i = coin; i <= (size_t)n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
}
