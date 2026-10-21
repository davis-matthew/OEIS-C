/*
 * A266747  G.f.:
 *          1/((1-t^7)^2*(1-t)*(1-t^3)*(1-t^5)*(1-t^9)*(1-t^11)*(1-t^13)).
 * https://oeis.org/A266747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in
 *            1/((1-t^7)^2*(1-t)*(1-t^3)*(1-t^5)*(1-t^9)*(1-t^11)*(1-t^13))
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const idx_t _parts[8] = {1, 3, 5, 7, 7, 9, 11, 13};

seq_t a(idx_t n)
{
    /* base case: the empty partition */
    if (n == 0) return (seq_t)1;

    /* Allocate DP array */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) abort();   /* allocation failure */

    /* Initialize dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }

    /* Unbounded knapsack DP: for each part weight */
    for (size_t pi = 0; pi < sizeof(_parts)/sizeof(_parts[0]); ++pi) {
        idx_t w = _parts[pi];
        for (idx_t j = w; j <= n; ++j) {
            dp[j] += dp[j - w];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
