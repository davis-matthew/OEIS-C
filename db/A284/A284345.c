/*
 * A284345  Number of partitions of n into squares dividing n.
 * https://oeis.org/A284345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into squares dividing n
 *            computed by an unlimited coin change DP over the set of square
 *            divisors of n. For n = 0 the value is 1. The function O(n *
 *            sqrt(n)) which easily satisfies the required limits
 * Range:     u8 n <= 107, u16 n <= 431, u32 n <= 2591, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper: integer floor square root for idx_t (uint64_t). */
static idx_t isqrt_idx(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 0.  For n == 0 the only
       partition is the empty one. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* 1. Build the list of square divisors of n. */
    idx_t sq_limit = isqrt_idx(n);
    /* at most sq_limit items */
    idx_t *sqs = (idx_t *)malloc((size_t)sq_limit * sizeof(idx_t));
    if (!sqs) return (seq_t)0;          /* Out of memory → 0 */
    size_t scnt = 0;

    for (idx_t d = 1; d <= sq_limit; ++d) {
        idx_t d2 = d * d;
        if (n % d2 == 0) {
            sqs[scnt++] = d2;
        }
    }

    /* 2. DP array for partition counts up to n. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) {
        free(sqs);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;      /* one way to make 0 */

    for (size_t si = 0; si < scnt; ++si) {
        idx_t coin = sqs[si];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }

    seq_t result = dp[n];

    free(dp);
    free(sqs);
    return result;
}
