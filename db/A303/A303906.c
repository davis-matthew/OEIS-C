/*
 * A303906  Expansion of Product_{k>=2} 1/(1 - x^(k*(k+1)/2)).
 * https://oeis.org/A303906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into triangular numbers > 1
 *            This is the coefficient of x^n in ∏_{k≥2} 1/(1 - x^{k(k+1)/2})
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined elsewhere in the build.          */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed partition numbers */
    static seq_t *dp = NULL;   /* dp[i] = a(i) for 0 ≤ i < dp_cap   */
    static idx_t  dp_cap = 0;  /* current size of dp (number of elements) */

    /* If we need a larger cache, recompute everything up to n.      */
    if (n >= dp_cap) {
        idx_t new_cap = n + 1;   /* number of elements needed */

        /* Reallocate array; abort on failure (should not happen). */
        seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!new_dp && new_cap != 0) abort();
        dp = new_dp;

        /* Initialise dp[0] = 1, all others to 0. */
        dp[0] = 1;
        for (idx_t i = 1; i < new_cap; ++i) dp[i] = 0;

        /* Dynamic-programming over all triangular numbers t = k(k+1)/2,
           for k ≥ 2 and t ≤ n.  For each t, add dp[i‑t] to dp[i].   */
        for (idx_t k = 2;; ++k) {
            idx_t t = (idx_t)(((seq_t)k * (seq_t)(k + 1)) / 2);
            if (t >= new_cap) break;          /* larger t will not affect dp[0..n] */
            for (idx_t i = t; i < new_cap; ++i)
                dp[i] += dp[i - t];
        }

        dp_cap = new_cap;   /* now dp has a valid value for all indices < new_cap */
    }

    return dp[n];
}
