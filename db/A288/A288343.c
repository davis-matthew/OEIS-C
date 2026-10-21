/*
 * A288343  Expansion of 1 / ((1-x)^2*(1-x^2)*(1-x^3)*...*(1-x^8)).
 * https://oeis.org/A288343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 208, u64 n <= 3609,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = sum_{k=0}^{n} P(k), where P(k) counts partitions
 * of k into parts ≤ 8 (equivalently, partitions of at most n into at most 8 parts). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array of length n+1 to hold the partition counts. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        abort();

    /* Initialise to zero. */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;                  /* one partition of 0: the empty partition */

    /* Count partitions of each integer k using parts 1..8. */
    for (idx_t part = 1; part <= 8; ++part)
    {
        for (idx_t j = part; j <= n; ++j)
        {
            dp[j] += dp[j - part];
        }
    }

    /* Convert to cumulative sums: a(n) = sum_{k=0}^{n} dp[k]. */
    for (idx_t j = 1; j <= n; ++j)
    {
        dp[j] += dp[j - 1];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
