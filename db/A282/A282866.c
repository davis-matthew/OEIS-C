/*
 * A282866  Expansion of Product_{k>=1} (1 + k^2*x^(k^2)).
 * https://oeis.org/A282866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A282866:
   a(n) = sum of products of the squares in all partitions of n into
   distinct squares.  Generating function: ∏_{k≥1} (1 + k²·x^{k²})   */

/* Standard includes */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/grow_seq.h"

/* Cached DP array: dp[s] = a(s) for all s <= max_cached. */
static seq_t *dp      = NULL;   /* array of coefficients */
static idx_t  max_cached = 0;   /* highest n that is already computed */
static size_t cap        = 0;   /* allocated capacity of dp */

/* Recompute the sequence up to (and including) 'up_to' using a 0‑1 knapsack DP. */
static void recompute(idx_t up_to)
{
    /* Ensure the buffer is large enough.  grow_seq() does a realloc if needed. */
    if (cap < (size_t)(up_to + 1))
        grow_seq(&dp, &cap, up_to + 1);

    /* Initialise all entries to 0.  The buffer may contain old data when
       cap has grown; zero out everything that will be used. */
    for (idx_t i = 0; i <= up_to; ++i)
        dp[i] = 0;
    dp[0] = 1;                /* empty partition contributes 1 */

    /* Number of square terms that can appear in a partition of 'up_to'. */
    idx_t kmax = (idx_t)sqrt((double)up_to);
    while ((kmax + 1) * (kmax + 1) <= up_to) ++kmax;
    while (kmax * kmax > up_to) --kmax;

    /* 0‑1 knapsack DP: for each square s, add it at most once. */
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t  s_val  = (seq_t)(k * k);   /* k²  stored as seq_t */
        idx_t  s_int  = k * k;            /* same value as a C integer for indices */

        for (idx_t j = up_to; j >= s_int; --j) {
            dp[j] += dp[j - s_int] * s_val;
        }
    }

    max_cached = up_to;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (dp == NULL || n > max_cached)
        recompute(n);

    return dp[n];
}
