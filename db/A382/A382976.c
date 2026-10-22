/*
 * A382976  Expansion of Product_{k>=1} (1 + (2^k + 1) * x^k).
 * https://oeis.org/A382976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k>=1} (1 + (2^k + 1) * x^k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP table */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return 0;          /* memory allocation failed */
    dp[0] = 1;              /* empty product contributes 1 */

    seq_t w = 3;            /* w = 2^1 + 1 initially */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update dp for part size k, using weight w */
        for (idx_t s = n; s >= k; --s) {
            seq_t prod = w * dp[s - k];
            dp[s] = dp[s] + prod;
        }
        /* Prepare weight for next part size */
        if (k < n) {
            w = (w * 2) - 1;   /* w_{k+1} = 2 * w_k - 1 */
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
