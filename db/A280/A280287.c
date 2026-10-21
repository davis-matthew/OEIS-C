/*
 * A280287  Number of partitions of n into distinct odd composite numbers
 *          (A071904).
 * https://oeis.org/A280287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into distinct odd composite numbers
 *            computed by a simple DP that iterates over all odd composite
 *            parts ≤ n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Allocate a DP table of size n+1 (index 0..n). */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* Allocation failure – return 0 as a safe value */

    /* Initialise the table: only sum 0 can be achieved initially. */
    for (size_t i = 0; i < len; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each odd composite number p ≤ n.
       Use a reverse loop to ensure each part is used at most once. */
    for (idx_t p = 3; p <= n; p += 2) {
        if (is_prime(p))
            continue;               /* skip odd primes */
        /* p is an odd composite number */
        for (idx_t s = n; s >= p; --s) {
            dp[s] += dp[s - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
