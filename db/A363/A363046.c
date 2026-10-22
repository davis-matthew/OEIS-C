/*
 * A363046  Number of partitions of n whose greatest part is a multiple of 4.
 * https://oeis.org/A363046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose greatest part is a
 *            multiple of 4 We compute this by dynamic programming: after each
 *            part m we know the number of partitions of each amount with
 *            parts not exceeding m. When m is a multiple of 4 the difference
 *            between the current and the previous DP value for...
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 141, u64 n <= 439,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                       /* for idx_t */
#include "../utils/seq_alloc_new_array.h" /* seq_alloc_new_array, seq_free_array */
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0) return 1;

    /* Allocate an array of seq_t that is correctly constructed */
    seq_t *dp = seq_alloc_new_array(n + 1);
    if (!dp) return 0;   /* o.o.m. fallback */

    /* Initialise dp[0] = 1, others 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    seq_t sum = 0;

    for (idx_t m = 1; m <= n; ++m) {
        seq_t before = dp[n];                 /* partitions with parts <= m-1 */
        for (idx_t i = m; i <= n; ++i) {
            dp[i] += dp[i - m];               /* include part m */
        }
        if ((m & 3) == 0) {                   /* m is divisible by 4 */
            sum += dp[n] - before;            /* partitions with largest part exactly m */
        }
    }

    seq_free_array(dp);
    return sum;
}
