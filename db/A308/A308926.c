/*
 * A308926  Sum of all the parts in the partitions of n into 7 parts.
 * https://oeis.org/A308926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p_7(n) where p_7(n) is the number of partitions of n
 *            into exactly 7 parts. Since each such partition sums to n, the
 *            total sum of all parts over all partitions equals n times the
 *            count of partitions. We compute p_7(n) via the identity p_7(n) =
 *            p_{≤7}(n-7) where p_{≤7}(m) counts partitio...
 * Range:     u8 n <= 14, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* If n < 7 there is no partition of n into 7 parts. */
    if (n < 7u)
        return (seq_t)0;

    /* m = n - 7 : count partitions of m into parts ≤ 7. */
    idx_t m = n - 7u;

    /* Allocate temporary array for DP: dp[0..m] */
    seq_t *dp = (seq_t *)malloc((m + 1u) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* memory failure – unlikely in tests */

    /* initialize dp[0] = 1, rest = 0 */
    memset(dp, 0, (m + 1u) * sizeof(seq_t));
    dp[0] = (seq_t)1;

    /* Unbounded knapsack DP over coin values 1..7 */
    for (idx_t value = 1u; value <= 7u; ++value) {
        for (idx_t sum = value; sum <= m; ++sum) {
            dp[sum] = dp[sum] + dp[sum - value];
        }
    }

    /* Number of partitions into exactly 7 parts */
    seq_t p7 = dp[m];

    /* Free temporary array */
    free(dp);

    /* Multiply by n (converted to seq_t) */
    return ((seq_t)n) * p7;
}
