/*
 * A270105  a(n) = Sum_{k=0..n} k*A000009(k).
 * https://oeis.org/A270105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270105: a(n) = Σ_{k=0..n} k*A000009(k)
   where A000009(k) = number of partitions of k into distinct parts. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached arrays: dp[i] = #partitions of i into distinct parts.
       prefix[i] = Σ_{k=0..i} k*dp[k] = a(i). */
    static seq_t *dp = NULL;
    static seq_t *prefix = NULL;
    static size_t cap = 0;          /* current allocated size (indexes 0..cap-1) */
    static idx_t   maxComputedN = 0;/* largest n for which dp and prefix are valid */

    /* Ensure arrays are large enough for indices 0..n. */
    if ((size_t)n + 1 > cap) {
        size_t newCap = (size_t)n + 1;
        dp      = (seq_t *)realloc(dp,      newCap * sizeof(seq_t));
        prefix  = (seq_t *)realloc(prefix,  newCap * sizeof(seq_t));
        /* Zero any newly added slots (prefix will be recomputed anyway). */
        for (size_t i = cap; i < newCap; ++i) {
            dp[i] = 0;
            prefix[i] = 0;
        }
        cap = newCap;
    }

    /* Recompute dp and prefix only when we need a larger n than we have yet. */
    if (n > maxComputedN) {
        /* Reset base of the partition DP. */
        dp[0] = 1;
        for (size_t i = 1; i <= (size_t)n; ++i)
            dp[i] = 0;

        /* Classic distinct‑part partition DP. */
        for (idx_t s = 1; s <= n; ++s) {
            for (idx_t j = n; j >= s; --j) {
                dp[j] = dp[j] + dp[j - s];
            }
        }

        /* Compute the prefix sums that yield the sequence values. */
        prefix[0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term = (seq_t)k * dp[k];
            prefix[k] = prefix[k - 1] + term;
        }
        maxComputedN = n;
    }

    return prefix[n];
}
