/*
 * A245574  Minimal coin changing sequence for denominations 1, 2, 5, 10, 20
 *          and 50 cents.
 * https://oeis.org/A245574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal coin changing for denominations 1, 2, 5, 10, 20, 50 cents.
   Computes a(n) = minimal number of coins needed to make n cents
   using dynamic programming with memoisation. */

#include <stdlib.h>

static const idx_t denoms[6] = {1, 2, 5, 10, 20, 50};

static seq_t *dp = NULL;      /* memoised DP table */
static idx_t dp_size = 0;     /* number of entries currently stored */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if (n + 1 > dp_size) {
        seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!new_dp) {              /* allocation failed */
            return 0;
        }
        dp = new_dp;
        idx_t old_size = dp_size;
        dp_size = n + 1;

        /* Initialise base case if the table was empty */
        if (old_size == 0) {
            dp[0] = (seq_t)0;
            old_size = 1;
        }

        /* Compute DP values from the first missing entry up to n */
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t best = (seq_t)(-1); /* maximum value for unsigned types */
            for (int k = 0; k < 6; ++k) {
                idx_t d = denoms[k];
                if (i >= d) {
                    seq_t val = dp[i - d] + (seq_t)1;
                    if (best == (seq_t)(-1) || val < best) {
                        best = val;
                    }
                }
            }
            dp[i] = best;
        }
    }

    return dp[n];
}
