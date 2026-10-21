/*
 * A277264  Expansion of Product_{k>=1} 1/(1 - x^(5*k+1)).
 * https://oeis.org/A277264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts larger than 1 *
 *            congruent to 1 modulo 5. (Products 1/(1 - x^(5*k+1)).)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The empty partition of 0 counts as 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate a DP array of size n+1 and zero‑initialize it. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) {
        return (seq_t)0;          /* out of memory – safe default */
    }
    dp[0] = (seq_t)1;             /* one way to make 0 using no parts */

    /* For each allowed part size (6, 11, 16, …) add its contribution. */
    for (idx_t part = 6; part <= n; part += 5) {
        for (idx_t sum = part; sum <= n; ++sum) {
            dp[(size_t)sum] += dp[(size_t)(sum - part)];
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
