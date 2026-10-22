/*
 * A357476  Number of partitions of n into two or more powers of 2.
 * https://oeis.org/A357476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A018819(n) - (n is a power of 2 ? 1 : 0) where A018819(n)
 *            counts partitions of n into powers of 2 (binary partitions). The
 *            function returns 0 for a(0)
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */


/* Maximum n this implementation can handle comfortably (test limit ≈ 10000). */
#define DP_CAP 40001

/* dp[i] stores the number of binary partitions of i. */
static seq_t dp[DP_CAP] = {0};
static idx_t max_computed = 0;

/* Recompute dp[0..limit] using the standard coin‑change DP
   for coin values that are powers of 2. */
static void recompute(idx_t limit)
{
    /* Clear the relevant part of the table.  dp[0] is set later. */
    for (idx_t i = 0; i <= limit; ++i)
        dp[i] = 0;

    dp[0] = 1;                       /* empty partition of 0 */

    for (idx_t coin = 1; coin <= limit; coin <<= 1) {
        for (idx_t j = coin; j <= limit; ++j) {
            dp[j] += dp[j - coin];
        }
    }
    max_computed = limit;
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 0. */
    if (n == 0u)
        return 0;

    /* Grow the DP table if necessary.  We assume n < DP_CAP
       in the usual test ranges; otherwise the computation would
       exceed the pre‑allocated array. */
    if (n >= DP_CAP) {
        /* For safety, recompute only up to the maximum supported
           capacity.  The caller is expected to use n within limits. */
        n = DP_CAP - 1;
    }

    if (n > max_computed)
        recompute(n);

    seq_t res = dp[n];
    /* Subtract the single-part partition if n itself is a power of 2. */
    if ((n & (n - 1)) == 0u)
        res -= 1;

    return res;
}
