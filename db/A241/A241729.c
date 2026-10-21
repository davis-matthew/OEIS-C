/*
 * A241729  Number of compositions of n into distinct parts with exactly ten
 *          descents.
 * https://oeis.org/A241729
 *
 * Offset:    66    (a(66) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥11} p_distinct(n,k) * A(k,10) where p_distinct(n,k)
 *            counts partitions of n into k distinct parts, and A(k,10) is the
 *            Eulerian number for k parts with exactly 10 descents
 * Range:     u8 n <= 77, u16 n <= 85, u32 n <= 112, u64 n <= 203,
 *            u128 n <= 551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute maximal number of distinct parts possible for this n */
    idx_t max_k = 0;
    for (idx_t k = 1; ; ++k) {
        if ( (k * (k + 1)) / 2 > n ) break;
        max_k = k;
    }

    /* dp[k][s] : partitions of s into k distinct parts
       flattened as dp[k * stride + s]  (stride = n+1)            */
    size_t stride = (size_t)(n + 1);
    size_t dp_size = (size_t)(max_k + 1) * stride;
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) return 0;                       /* out of memory → 0 */

    /* base case: zero parts sum to zero */
    dp[0] = 1;

    /* Fill dp using recurrence:
         p(k,s) = p(k-1, s-k) + p(k, s-k)    (for s ≥ k)
         (recurrence used shifts the smallest part by 1)   */
    for (size_t cur_s = 1; cur_s <= n; ++cur_s) {
        for (idx_t k = 1; k <= max_k; ++k) {
            seq_t val = 0;
            if (cur_s >= (size_t)k) {
                val += dp[((size_t)k   * stride) + (cur_s - k)];
                val += dp[((size_t)(k-1) * stride) + (cur_s - k)];
            }
            dp[((size_t)k * stride) + cur_s] = val;
        }
    }

    /* Eulerian numbers A(k,10) up to max_k */
    seq_t *euler = (seq_t *)calloc((size_t)(max_k + 1), sizeof(seq_t));
    if (!euler) {
        free(dp);
        return 0;
    }

    /* recurrence: A(k,m) = (k-m) * A(k-1,m-1) + (m+1) * A(k-1,m) */
    seq_t prev[11] = {0};
    prev[0] = 1;   /* A(0,0) = 1 */

    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t cur[11];
        cur[0] = 1;               /* A(k,0) = 1 */
        for (idx_t m = 1; m <= 10; ++m) {
            seq_t t1 = (seq_t)(k - m) * prev[m - 1];
            seq_t t2 = (seq_t)(m + 1) * prev[m];
            cur[m] = t1 + t2;
        }
        euler[k] = cur[10];
        for (idx_t m = 0; m <= 10; ++m) prev[m] = cur[m];
    }

    /* sum contributions for all k ≥ 11 */
    seq_t result = 0;
    if (max_k >= 11) {
        for (idx_t k = 11; k <= max_k; ++k) {
            seq_t ways = dp[((size_t)k * stride) + n];
            result += ways * euler[k];
        }
    }

    free(dp);
    free(euler);
    return result;
}
