/*
 * A319818  Number of partitions of n into exactly eight positive triangular
 *          numbers.
 * https://oeis.org/A319818
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly eight positive
 *            triangular numbers
 * Range:     u8 n <= 144, u16 n <= 1173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are provided by the build system */
seq_t a(idx_t n)
{
    /* The offset is 8; a(8) is the first non‑zero term. */
    if (n < 8) return 0;

    /* Find all triangular numbers T(j) = j(j+1)/2 that do not exceed n.
       Store them as idx_t values – they are all <= n and fit in a 64‑bit integer.   */
    idx_t max_t = 1;
    while (((seq_t)max_t * (max_t + 1)) / 2 <= n) max_t++;
    /* max_t now points to first j with T(j) > n, so there are max_t-1 triangulars. */
    size_t tri_cnt = (size_t)(max_t - 1);

    /* Allocate DP table: (n+1) possible sums × 9 possible counts (0..8).  */
    size_t dp_len = (size_t)(n + 1) * 9;
    seq_t *dp = (seq_t *)calloc(dp_len, sizeof(seq_t));
    if (!dp) return 0;                 /* out of memory – silently fail */

    /* Macro for accessing dp[sum][k] as a flat array.  */
    #define DP(sum, k) dp[((size_t)(sum)) * 9 + (k)]

    DP(0, 0) = 1;                       /* empty sum uses zero triangulars */

    /* Main DP: unbounded‑knapsack with an extra dimension for the number of items. */
    for (size_t ti = 0; ti < tri_cnt; ++ti) {
        idx_t t = (idx_t)(ti + 1);                 /* j = 1..max_t-1   */
        seq_t tri_seq = ((seq_t)t * (t + 1)) / 2;  /* compute T(j) in seq_t */
        idx_t tri = (idx_t)tri_seq;               /* safe: tri_seq <= n */

        for (idx_t sum = tri; sum <= n; ++sum) {
            for (int k = 1; k <= 8; ++k) {
                DP(sum, k) += DP(sum - tri, k - 1);
            }
        }
    }

    seq_t result = DP(n, 8);
    free(dp);
    #undef DP
    return result;
}
