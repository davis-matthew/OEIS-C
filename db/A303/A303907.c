/*
 * A303907  Expansion of Product_{k>=2} (1 + x^(k*(k+1)/2)).
 * https://oeis.org/A303907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of triangular numbers > 1 summing to n
 * Range:     u8 n <= 367, u16 n <= 1244, u32 n <= 5323, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distinct partitions of n into triangular numbers k*(k+1)/2 with k >= 2 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate array dp[0..n] and initialise to 0 */
    seq_t *dp = NULL;
    /* Use calloc so that all elements are zero-initialised (works for
       integral seq_t types, and for big‑int types that are trivially
       constructible). */
    dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* allocation failure, return 0 */

    dp[0] = (seq_t)1;   /* one way to form 0: use no triangular numbers */

    /* Iterate over triangular numbers > 1: t = k*(k+1)/2, k >= 2 */
    for (idx_t k = 2; ; ++k) {
        idx_t t = k * (k + 1) / 2;   /* integer arithmetic, fits in idx_t */
        if (t > n) break;
        /* Update dp from high to low to avoid using a number more than once */
        for (idx_t s = n; s >= t; --s) {
            dp[s] += dp[s - t];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
