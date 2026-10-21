/*
 * A292306  a(n) = [x^n] Product_{k>=1} (1 + n^n*x^k).
 * https://oeis.org/A292306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] Product_{k>=1} (1 + n^n * x^k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* C = n^n  (n is idx_t, cast to seq_t) */
    seq_t C = big_pow((seq_t)n, (seq_t)n);

    /* Allocate array dp[0..n] for coefficients of x^j */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* out of memory */

    dp[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j)
        dp[j] = (seq_t)0;

    /* Distinct part DP */
    for (idx_t k = 1; k <= n; ++k) {
        /* back‑to‑front to avoid reusing part k */
        for (idx_t j = n; j >= k; --j) {
            dp[j] += dp[j - k] * C;
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
