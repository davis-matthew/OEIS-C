/*
 * A300982  Number of partitions of n into parts having the same number of
 *          prime divisors (counted with multiplicity) as n.
 * https://oeis.org/A300982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] ∏_{bigomega(k)=bigomega(n)} 1/(1 - x^k)
 * Range:     u8 n <= 40, u16 n <= 108, u32 n <= 358, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

/* Count of partitions of n into parts that have the same number of
   prime divisors (counted with multiplicity) as n. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Compute the common omega value */
    seq_t w = bigomega(n);

    /* Allocate DP array of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* allocation failure (unlikely for n ≤ 10000) */

    /* Initialise DP */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Update DP for each part k with bigomega(k) == w */
    for (idx_t k = 1; k <= n; ++k) {
        if (bigomega(k) == w) {
            for (idx_t j = k; j <= n; ++j)
                dp[j] += dp[j - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
