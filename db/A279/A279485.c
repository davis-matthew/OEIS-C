/*
 * A279485  Expansion of Product_{k>=1} (1-x^(k^4)).
 * https://oeis.org/A279485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^{k^4})
 * Range:     i8 n <= 9268, i16 n <= 9268, i32 n <= 9268, i64 n <= 9268,
 *            i128 n <= 9268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1 and zero it. */
    idx_t sz = n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) return (seq_t)0;   /* fallback in case of OOM */

    /* dp[0] = 1 – empty sum, zero elements (even size) */
    dp[0] = (seq_t)1;

    /* Iterate over fourth powers k^4 <= n. */
    for (idx_t k = 1; ; ++k) {
        /* Compute k^4 safely, breaking if it exceeds n. */
        idx_t v = k * k;
        if (v > n) break;
        v = v * v;
        if (v > n) break;

        /* Update DP array in reverse to avoid reuse of updated entries. */
        for (idx_t i = n; i >= v; --i) {
            dp[i] = dp[i] - dp[i - v];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
