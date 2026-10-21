/*
 * A276236  Number of triangular partitions of n of order 7.
 * https://oeis.org/A276236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            1/((1-x)^7*(1-x^3)^6*(1-x^5)^5*(1-x^7)^4*(1-x^9)^3*(1-x^11)^2*(1-x^13))
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate DP array */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failure – unlikely for n <= 10000 */
    dp[0] = (seq_t)1;            /* empty partition */

    /* coin sizes (k) and number of colours (m) for each factor */
    const idx_t vals[] = {1, 3, 5, 7, 9, 11, 13};
    const idx_t exps[] = {7, 6, 5, 4, 3, 2, 1};
    const size_t num_vals = sizeof(vals) / sizeof(vals[0]);

    for (size_t j = 0; j < num_vals; ++j) {
        idx_t v = vals[j];
        idx_t m = exps[j];
        size_t step = (size_t)v;
        for (idx_t t = 0; t < m; ++t) {        /* each colour separately */
            for (size_t sum = step; sum <= N; ++sum) {
                dp[sum] += dp[sum - step];
            }
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
