/*
 * A278949  Expansion of Product_{k>=1} 1/(1 - x^(k*(2*k-1))).
 * https://oeis.org/A278949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into nonzero hexagonal numbers
 *            (OEIS A278949, a(0)=1). The generating function is ∏_{k≥1} 1/(1
 *            - x^{k(2k-1)}). We compute the coefficient of x^n by a standard
 *            coin‑change DP with the hexagonal numbers as coin values
 * Range:     u8 n <= 112, u16 n <= 408, u32 n <= 1817, u64 n <= 9640,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate a table dp[0..n] initialized to 0.  dp[s] will hold
       the number of partitions of s into hexagonal numbers. */
    size_t size = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(size, sizeof(seq_t));
    if (!dp) return (seq_t)0;
    dp[0] = 1;  /* base case: one way to partition 0 */

    /* For each hexagonal number h = k(2k−1) not exceeding n,
       update the table in the standard unbounded‑knapsack way. */
    for (idx_t k = 1;; ++k) {
        idx_t h = k * (2 * k - 1);
        if (h > n) break;
        for (idx_t s = h; s <= n; ++s) {
            dp[s] += dp[s - h];
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
