/*
 * A261742  Number of partitions of n where each part i is marked with a word
 *          of length i over an octonary alphabet whose letters appear in
 *          alphabetical order.
 * https://oeis.org/A261742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261742
   a(n) = coefficient of x^n in  ∏_{k≥1} 1/(1 - binomial(k+7,7)·x^k)
   (Number of partitions of n where each part i is marked with a word of
   length i over an octonary alphabet whose letters appear in alphabetical
   order.)                                         
*/

#include <stdlib.h>
#include <stdint.h>

/* Return binomial(k+7,7) as a seq_t.  The calculation is done
   multiplicatively so that it works for all seq_t sizes. */
static seq_t binom7(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        r = r * (k + i);   /* (k+7 + i) = (k + i)  (i=1..7) */
        r = r / i;         /* exact division */
    }
    return r;
}

/* Static cache to avoid reallocating each call.  The array size grows
   only when a larger n is requested. */
static seq_t *dp = NULL;       /* array of dp[0..n] */
static size_t dp_cap = 0;      /* allocated capacity (dp_cap = dp_size) */

/* Main function.  The implementation uses the naive O(n²) dynamic
   programming algorithm described in the comment.  It recomputes the
   entire table for every call, which satisfies the required correctness
   and performance for n up to ~10000. */
seq_t a(idx_t n)
{
    /* Ensure we have an array large enough.  N+1 entries are needed. */
    if ((size_t)n + 1 > dp_cap) {
        size_t new_cap = ((size_t)n + 1);
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!new_dp) {   /* allocation failure – return zero as safe default */
            return (seq_t)0;
        }
        dp = new_dp;
        dp_cap = new_cap;
    }

    /* Initialise dp[0] and clear the rest. */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Dynamic programming to build all coefficients up to n. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t w = binom7(k);                    /* weight for part k */
        for (idx_t m = k; m <= n; ++m) {
            dp[m] += dp[m - k] * w;
        }
    }

    return dp[n];
}
