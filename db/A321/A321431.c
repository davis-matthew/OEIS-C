/*
 * A321431  Expansion of Product_{i>0, j>0} 1/(1 - x^(i^2 + j^2)).
 * https://oeis.org/A321431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{i>0,j>0} 1/(1-x^(i^2+j^2))
 * Range:     u8 n <= 34, u16 n <= 84, u32 n <= 229, u64 n <= 696,
 *            u128 n <= 2297, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A321431: number of partitions of n into parts each equal to a sum of two positive squares, with each ordered pair counted as a distinct part type. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed coefficients. */
    static seq_t *dp = NULL;          /* DP array for coefficients 0..max_n */
    static size_t max_n = 0;          /* maximum n for which dp has been filled */

    size_t N = (size_t)n;

    /* If out of range, recompute from scratch. */
    if (dp == NULL || N > max_n) {
        /* Allocate fresh DP array. */
        seq_t *newdp = (seq_t*)calloc(N + 1, sizeof(seq_t));
        if (!newdp) return (seq_t)0; /* Out of memory: return 0 as fallback. */
        newdp[0] = (seq_t)1;          /* a(0) = 1 */

        /* Generate all ordered pairs (i,j) with i,j >= 1 and i^2 + j^2 <= N
           and perform the coin–change DP updates. */
        for (idx_t i = 1; (idx_t)i * i <= n; ++i) {
            idx_t i2 = (idx_t)i * i;
            for (idx_t j = 1; i2 + (idx_t)j * j <= n; ++j) {
                idx_t s = i2 + (idx_t)j * j;
                size_t sv = (size_t)s;
                for (size_t v = sv; v <= N; ++v) {
                    newdp[v] += newdp[v - sv];
                }
            }
        }

        /* Replace old DP array. */
        free(dp);
        dp = newdp;
        max_n = N;
    }

    /* Return the computed coefficient. */
    return dp[N];
}
