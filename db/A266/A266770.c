/*
 * A266770  Molien series for invariants of finite Coxeter group D_7.
 * https://oeis.org/A266770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in
 *            1/((1-x^2)(1-x^4)(1-x^6)(1-x^7)(1-x^8)(1-x^10)(1-x^12)).
 *            Computed by DP over the allowed part sizes: 2,4,6,7,8,10,12
 * Range:     u8 n <= 37, u16 n <= 131, u32 n <= 971, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate DP array of size n+1 */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp) return 0;
    dp[0] = (seq_t)1;

    static const idx_t parts[7] = {2, 4, 6, 7, 8, 10, 12};
    const size_t num_parts = 7;

    for (size_t pi = 0; pi < num_parts; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
