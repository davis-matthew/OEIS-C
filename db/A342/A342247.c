/*
 * A342247  Number of partitions of n into seven powers of 2.
 * https://oeis.org/A342247
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into seven powers of 2 (each part 1,2,4,8,…) */
/* Computes a(n) for any n ≥ 7 using an O(number_of_denoms · 7 · n) DP. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 7 no partition with exactly 7 parts exists. */
    if (n < 7) {
        return (seq_t)0;
    }

    /* Number of parts fixed */
    const idx_t parts = 7;
    /* Base for linear index: size of sum dimension */
    const idx_t base = n + 1;

    /* Allocate DP array: (parts+1) * (n+1) elements of type seq_t. */
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (parts + 1) * base);
    if (!dp) {
        /* In case of allocation failure, return 0 (should not happen). */
        return (seq_t)0;
    }

    /* Helper macro for linear indexing into dp[p][s] where 0 ≤ p ≤ parts, 0 ≤ s ≤ n. */
    #define DP(p, s) (dp[(size_t)(p) * base + (s)])

    /* Initialize: zero all entries. */
    for (size_t i = 0; i < (size_t)(parts + 1) * (size_t)base; ++i) {
        dp[i] = (seq_t)0;
    }
    /* There is exactly one way to make sum 0 with 0 parts. */
    DP(0, 0) = (seq_t)1;

    /* For each coin (power of two) process DP. */
    for (idx_t coin = 1; coin <= n; coin <<= 1) {
        /* For each possible number of parts used so far (at least 1). */
        for (idx_t p = 1; p <= parts; ++p) {
            /* Update sums in increasing order to allow unlimited use of this coin. */
            for (idx_t s = coin; s <= n; ++s) {
                /* dp[p][s] adds ways using one more coin of value 'coin', from dp[p-1][s-coin]. */
                DP(p, s) += DP(p - 1, s - coin);
            }
        }
    }

    /* Result is number of ways to form n with exactly 7 parts. */
    seq_t result = DP(parts, n);

    free(dp);
    return result;
}
