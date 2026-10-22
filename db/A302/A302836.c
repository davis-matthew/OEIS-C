/*
 * A302836  a(n) = [x^n] Product_{k>=1} 1/(1 - n*x^(k^2)).
 * https://oeis.org/A302836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - n·x^{k^2})⁻¹ This is
 *            computed by dynamic programming: a(n) = Σ_{partitions of n into
 *            squares} n^{#parts}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Allocate array dp[0..n], initialize to 0 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;            /* out of memory fallback */
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;                    /* empty partition */

    /* Process each square t² ≤ n */
    for (idx_t t = 1; t * t <= n; ++t) {
        idx_t t2 = t * t;
        seq_t mult = (seq_t)n;           /* factor n for each use of t² */
        for (idx_t s = t2; s <= n; ++s) {
            dp[s] += dp[s - t2] * mult;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
