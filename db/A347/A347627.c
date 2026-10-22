/*
 * A347627  Number of partitions of n into at most 4 triangular numbers.
 * https://oeis.org/A347627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into at most 4 triangular numbers */
#include <stdlib.h>
#include <stdint.h>

/* dp[p][s] = number of partitions of s using exactly p triangular numbers
 * for p = 0..4, s = 0..n.
 */
seq_t a(idx_t n)
{
    /* allocate DP tables */
    seq_t *dp[5];
    size_t sz = (size_t)(n + 1);                /* include index n */
    for (int p = 0; p < 5; ++p) {
        dp[p] = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp[p]) {
            /* failure: free already allocated tables and return 0 */
            for (int q = 0; q < p; ++q) free(dp[q]);
            return 0;
        }
    }

    dp[0][0] = 1;      /* empty partition */

    /* iterate over triangular numbers t = k(k+1)/2, k>=1, t <= n */
    for (idx_t k = 1; ; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;

        for (idx_t s = t; s <= n; ++s) {
            for (int p = 1; p <= 4; ++p) {
                dp[p][s] += dp[p - 1][s - t];
            }
        }
    }

    /* sum partitions with 0..4 parts */
    seq_t result = dp[0][n];
    for (int p = 1; p <= 4; ++p) result += dp[p][n];

    /* free memory */
    for (int p = 0; p < 5; ++p) free(dp[p]);

    return result;
}
