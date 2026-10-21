/*
 * A279223  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)*(5*k-2)/6)).
 * https://oeis.org/A279223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A279223:  number of partitions of n into heptagonal
    pyramidal numbers  h(k) = k*(k+1)*(5*k-2)/6.  The value a(n) is
    obtained with the classic unbounded‑knapsack DP.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate and initialise DP table. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp)                 /* unlikely, but keep code safe */
        return (seq_t)0;
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* Generate all heptagonal pyramidal numbers up to n and
       perform the standard unbounded‑knapsack DP. */
    for (idx_t k = 1; ; ++k) {
        idx_t h = k * (k + 1) * (5 * k - 2) / 6;
        if (h > n) break;
        for (idx_t i = h; i <= n; ++i)
            dp[i] += dp[i - h];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
