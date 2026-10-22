/*
 * A385010  G.f.: Product_{k>=1} (1 + x^(3*k^2)) / (1 - x^k).
 * https://oeis.org/A385010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 40, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A385010:
 *   Generating function: ∏_{k≥1} (1 + x^(3k^2)) / (1 - x^k)
 *   a(n) is the coefficient of x^n in the above product.
 *   We compute it by dynamic programming over parts k, treating
 *   the unlimited part 1/(1-x^k) and the optional special part
 *   (1 + x^(3k^2)) separately.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate and zero-initialize DP array of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (dp == NULL)
        return (seq_t)0;  /* allocation failed; return 0 as a fallback */

    dp[0] = (seq_t)1;  /* empty partition */

    for (idx_t k = 1; k <= n; ++k) {
        /* --------------------------------------------------------------------
         *  Add unlimited copies of part k: 1/(1-x^k)
         * -------------------------------------------------------------------- */
        for (idx_t i = k; i <= n; ++i)
            dp[i] += dp[i - k];

        /* --------------------------------------------------------------------
         *  Add optional single special part of size 3k^2: (1 + x^(3k^2))
         *  When w = 3k^2 <= n, we add dp[i-w] to dp[i] for all i >= w.
         * -------------------------------------------------------------------- */
        idx_t w = 3 * k * k;
        if (w <= n) {
            for (idx_t i = n; i >= w; --i)
                dp[i] += dp[i - w];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
