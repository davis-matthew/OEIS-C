/*
 * A282502  Expansion of 1/(1 - Sum_{k>=0} x^(3*k*(k+1)/2+1)).
 * https://oeis.org/A282502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into centered triangular
 *            numbers (widest expression: a(0)=1; for n>0, a(n)=∑_{k≥0,
 *            3k(k+1)/2+1≤n} a(n-(3k(k+1)/2+1)) )
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* for size_t */
#include "../utils/seq_realloc.h"  /* seq_realloc */

seq_t a(idx_t n)
{
    /* Static DP table and bookkeeping */
    static seq_t *dp = NULL;
    static size_t capacity = 0;   /* allocated size of dp */
    static idx_t computed = 0;    /* one past the largest index we have computed */

    /* Ensure enough capacity for index n */
    if (capacity <= (size_t)n) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        dp = seq_realloc(dp, capacity, new_cap);
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            dp[0] = 1;                      /* base case */
            continue;
        }
        dp[i] = 0;                          /* accumulator */

        /* iterate over centered triangular numbers t_k ≤ i */
        for (idx_t k = 0; ; ++k) {
            idx_t t = 3 * k * (k + 1) / 2 + 1;   /* t_k */
            if (t > i) break;
            dp[i] += dp[i - t];
        }
    }

    computed = n + 1;   /* we now have values up to n */
    return dp[n];
}
