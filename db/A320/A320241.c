/*
 * A320241  Expansion of Product_{k=1..9} theta_3(q^k), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A320241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of integer solutions (a₁,…,a₉) to a₁² + 2·a₂² + …
 *            + 9·a₉² = n
 * Range:     u8 n <= 14, u16 n <= 68, u32 n <= 1598, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Two work arrays of length n+1.  They are allocated once and swapped. */
    idx_t size = n + (idx_t)1;

    seq_t *dp   = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp) return (seq_t)0;
    seq_t *next = (seq_t *)calloc(size, sizeof(seq_t));
    if (!next) { free(dp); return (seq_t)0; }

    dp[0] = (seq_t)1;          /* empty assignment yields sum 0 */

    for (idx_t k = 1; k <= 9; ++k) {
        /* dependences of previous step are in dp,
           results for current k will be accumulated in next */
        for (idx_t i = 0; i < size; ++i) next[i] = (seq_t)0;

        for (idx_t sum = 0; sum <= n; ++sum) {
            seq_t ways = dp[sum];
            if (!ways) continue;

            idx_t remaining = n - sum;
            for (idx_t a = 0; ; ++a) {
                idx_t contrib = k * a * a;
                if (contrib > remaining) break;
                idx_t t = sum + contrib;
                if (a == 0)
                    next[t] += ways;            /* weight 1 for a = 0 */
                else
                    next[t] += ways + ways;      /* weight 2 for a ≠ 0 */
            }
        }
        /* Swap dp and next for the next iteration. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(next);
    return result;
}
