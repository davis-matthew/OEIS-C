/*
 * A301565  Expansion of Product_{k>=0} (1 + x^(5*k+3))*(1 + x^(5*k+4)).
 * https://oeis.org/A301565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into distinct parts congruent to 3 or 4 mod 5.
   The value a(n) is the number of subsets of {3,4,8,9,13,14,…} that sum to n. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Convert index to a size_t for array indexing. */
    size_t N = (size_t)n;

    /* Allocate the DP array. */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (dp == NULL) return 0;      /* allocation failure, return 0 */

    /* Initialise dp[0] = 1, others 0. */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) dp[i] = (seq_t)0;

    /* Process parts congruent to 3 mod 5. */
    for (size_t p = 3; p <= N; p += 5) {
        for (size_t s = N; s >= p; --s) {
            dp[s] += dp[s - p];
        }
    }

    /* Process parts congruent to 4 mod 5. */
    for (size_t p = 4; p <= N; p += 5) {
        for (size_t s = N; s >= p; --s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
