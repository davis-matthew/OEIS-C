/*
 * A308400  Expansion of 1 / Sum_{k=-oo..oo} (-x)^(k*(6*k + 1)).
 * https://oeis.org/A308400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to {0,5,7}
 *            mod 12
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the partition number a(n).  The function keeps a static DP
 * array up to the maximum argument seen so far, so repeated calls are
 * efficient even when calls are in arbitrary order. */
seq_t a(idx_t n)
{
    /* Static cache of dynamic programming values up to the current maximum n. */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;   /* number of elements currently allocated (dp_size = max_n + 1) */

    /* If the cache is uninitialized or too small, reallocate and recompute. */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp) {
            /* Allocation failed – return 0 as a safe fallback. */
            return (seq_t)0;
        }

        /* Initialize DP table. */
        new_dp[0] = (seq_t)1;
        for (idx_t i = 1; i < new_size; ++i) {
            new_dp[i] = (seq_t)0;
        }

        /* Add all coins (allowed parts) up to the new maximum. */
        /* Coin types: multiples of 12 (12,24,36,...)   */
        for (idx_t m = 1; 12 * m <= n; ++m) {
            idx_t c = 12 * m;
            for (idx_t i = c; i <= n; ++i) {
                new_dp[i] += new_dp[i - c];
            }
        }
        /* Coins congruent to 5 mod 12 (5,17,29,...)  */
        for (idx_t m = 0; 12 * m + 5 <= n; ++m) {
            idx_t c = 12 * m + 5;
            for (idx_t i = c; i <= n; ++i) {
                new_dp[i] += new_dp[i - c];
            }
        }
        /* Coins congruent to 7 mod 12 (7,19,31,...)  */
        for (idx_t m = 0; 12 * m + 7 <= n; ++m) {
            idx_t c = 12 * m + 7;
            for (idx_t i = c; i <= n; ++i) {
                new_dp[i] += new_dp[i - c];
            }
        }

        /* Replace old table. */
        free(dp);
        dp = new_dp;
        dp_size = new_size;
    }

    return dp[n];
}
