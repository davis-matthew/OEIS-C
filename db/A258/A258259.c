/*
 * A258259  Number of partitions of n into distinct parts less than or equal
 *          to n/2.
 * https://oeis.org/A258259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,…,⌊n/2⌋} summing to n
 * Range:     u8 n <= 32, u16 n <= 79, u32 n <= 237, u64 n <= 791,
 *            u128 n <= 2821, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e., the coefficient of x^n in ∏_{i=1..⌊n/2⌋} (1 + x^i)          */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the trivial case of n = 0 */
    if (n == 0u) {
        /* a(0) = 1 by definition */
        return (seq_t)1;
    }

    /* allocate DP array of size n+1, initialized to 0 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* fallback, should not happen */

    dp[0] = (seq_t)1;   /* empty subset sums to 0 */

    /* iterate over possible part sizes i = 1 .. floor(n/2) */
    for (idx_t i = 1; i <= n / 2; ++i) {
        /* Update dp[s] for sums s = n .. i (descending to avoid reuse of i) */
        for (idx_t s = n; s >= i; --s) {
            dp[s] += dp[s - i];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
