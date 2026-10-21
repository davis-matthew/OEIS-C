/*
 * A276559  Expansion of Sum_{k>=1} k^2*x^k^2/(1 - x^k^2) * Product_{k>=1}
 *          1/(1 - x^k^2).
 * https://oeis.org/A276559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A001156(n) (partitions of n into squares)
 * Range:     u8 n <= 20, u16 n <= 89, u32 n <= 493, u64 n <= 3172,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function computes the number of partitions of n into squares (A001156)
    using a dynamic programming approach and returns n multiplied by that count. */

#include <stdlib.h>

static seq_t *dp = NULL;      /* working array for partition counts */
static size_t  dp_cap = 0;    /* current capacity of dp[ ] */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; 0 is undefined */

    /* Ensure dp has space for indices 0..n */
    size_t size = (size_t)n + 1;
    if (size > dp_cap) {
        seq_t *tmp = (seq_t *)realloc(dp, size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure → 0 */
        dp = tmp;
        dp_cap = size;
    }

    /* Reset dp[0..n] to zero */
    for (size_t i = 0; i < size; ++i)
        dp[i] = (seq_t)0;

    dp[0] = (seq_t)1;   /* one way to partition 0 (empty partition) */

    /* dp[i] = number of partitions of i into squares */
    for (idx_t s = 1; (idx_t)(s * s) <= n; ++s) {
        idx_t square = s * s;
        for (idx_t i = square; i <= n; ++i)
            dp[i] += dp[i - square];
    }

    return (seq_t)n * dp[n];
}
