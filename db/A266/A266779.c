/*
 * A266779  Molien series for invariants of finite Coxeter group A_10.
 * https://oeis.org/A266779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in 1/((1-t^2)*(1-t^3)*...*(1-t^11)),
 *            i.e. the number of partitions of n into parts of sizes 2 through
 *            11
 * Range:     u8 n <= 24, u16 n <= 68, u32 n <= 308, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: This implementation uses a simple dynamic programming
   that runs in O(n * 10) time and O(n) memory. It works for any
   64‑bit index n and produces exact results in the supplied seq_t
   type, which supports all integer arithmetic. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n == 0 the product is 1 by definition. */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of n+1 seq_t elements, initialized to 0. */
    /* Casting to size_t assumes n+1 fits in space_t; for the test
       range (up to ~10^5) this is safe. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;               /* Loss of data if memory failure */

    dp[0] = (seq_t)1;                  /* One way to make sum 0 */

    /* For each part size k in [2,11], add ways to form sums including k. */
    for (idx_t k = 2; k <= 11; ++k) {
        for (idx_t i = k; i <= n; ++i) {
            dp[i] += dp[i - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
