/*
 * A322855  Number of compositions (ordered partitions) of n into heptagonal
 *          pyramidal numbers (A002413).
 * https://oeis.org/A322855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322855: Number of compositions of n into heptagonal pyramidal numbers
   G.f.: 1/(1 - Sum_{k>=1} x^(k*(k+1)*(5*k-2)/6)) */

#include <stdlib.h>
#include <stdint.h>

/* Computes a(n) */
seq_t a(idx_t n)
{
    /* Dynamic programming array for the sequence. */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;      /* allocated capacity of dp         */
    static idx_t computed = 0;    /* highest index already computed   */

    /* Grow the array if necessary. */
    if (n + 1 > dp_cap) {
        idx_t new_cap = n + 1;
        if (dp_cap > 0 && new_cap < dp_cap * 2)
            new_cap = dp_cap * 2;
        seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_dp)
            return (seq_t)0; /* allocation failure – unlikely in tests */

        /* Copy old contents. */
        if (dp) {
            for (idx_t i = 0; i < dp_cap; ++i)
                new_dp[i] = dp[i];
            free(dp);
        }

        /* Initialise the new slots to zero. */
        for (idx_t i = dp_cap; i < new_cap; ++i)
            new_dp[i] = (seq_t)0;

        dp = new_dp;
        dp_cap = new_cap;
    }

    /* Base case: dp[0] = 1. */
    if (dp[0] == (seq_t)0)
        dp[0] = (seq_t)1;

    /* Compute new entries up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            /* Compute the k-th heptagonal pyramidal number:
               s_k = k*(k+1)*(5*k-2)/6  */
            seq_t s = (seq_t)k;
            s *= (seq_t)(k + 1);
            s *= (seq_t)(5 * k - 2);
            s /= (seq_t)6;
            if (s > (seq_t)i)
                break;
            dp[i] += dp[i - (idx_t)s];
        }
        computed = i;
    }

    return dp[n];
}
