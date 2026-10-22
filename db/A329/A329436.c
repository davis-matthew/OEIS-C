/*
 * A329436  Expansion of Sum_{k>=1} (-1 + Product_{j>=2} (1 + x^(k*j))).
 * https://oeis.org/A329436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A025147(d) where A025147(d) = number of
 *            partitions of d into distinct parts >= 2 (i.e. partitions of d
 *            with all parts distinct and none equal to 1)
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* Compute the sequence value a(n) for n >= 1. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined offset, but safe */

    /* static caches */
    static seq_t *dp = NULL;
    static seq_t *a_cache = NULL;
    static idx_t precomp = 0;

    /* Recompute caches up to n if necessary */
    if (n > precomp) {
        idx_t N = n;
        /* Allocate new arrays of size N+1 (index 0..N) */
        seq_t *new_dp = alloc_seq_array(N + 1);
        seq_t *new_a  = alloc_seq_array(N + 1);

        /* Initialize dp[0] = 1 */
        new_dp[0] = (seq_t)1;

        /* Compute partitions into distinct parts >= 2
           using classic coin‑change DP (each part at most once) */
        for (idx_t coin = 2; coin <= N; ++coin) {
            for (idx_t i = N; i >= coin; --i) {
                new_dp[i] += new_dp[i - coin];
            }
        }

        /* Initialize a_cache to zero (alloc_seq_array should zero it
           but we do it explicitly for safety) */
        for (idx_t i = 1; i <= N; ++i)
            new_a[i] = 0;

        /* a(n) is the divisor sum of dp: a_cache[m] = Σ_{d|m} dp[d]   */
        for (idx_t d = 1; d <= N; ++d) {
            seq_t val = new_dp[d];
            for (idx_t m = d; m <= N; m += d) {
                new_a[m] += val;
            }
        }

        /* Free old caches if they exist */
        if (dp)     free(dp);
        if (a_cache) free(a_cache);

        /* Install new caches */
        dp       = new_dp;
        a_cache  = new_a;
        precomp  = N;
    }

    /* Return precomputed value */
    return a_cache[n];
}
