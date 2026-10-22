/*
 * A304782  a(n) = [x^n] (1/(1 - x))*Product_{k>=1} (1 + n*x^k).
 * https://oeis.org/A304782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (1/(1 - x))*Product_{k>=1} (1 + n*x^k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Direct O(n^2) computation using dynamic programming.
       dp[m] = sum over subsets S of {1..n} with total size m of n^{|S|}.
       Then a(n) = sum_{m=0}^{n} dp[m]. */
    if (n == 0) return 1;

    size_t N = (size_t)n;
    /* dp array (size N+1) */
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = 1;

    seq_t seqN = (seq_t)n;

    for (size_t k = 1; k <= N; ++k) {
        for (size_t m = N; m >= k; --m) {
            dp[m] += dp[m - k] * seqN;
        }
    }

    /* prefix sum to produce final a(n) */
    seq_t sum = 0;
    for (size_t m = 0; m <= N; ++m) {
        sum += dp[m];
    }

    free(dp);
    return sum;
}
