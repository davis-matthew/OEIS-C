/*
 * A262050  Expansion of f(-x)^2 * f(-x^10) / phi(-x)^3 in powers of x where
 *          phi(), f() are Ramanujan theta functions.
 * https://oeis.org/A262050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 – x^k)^(-b(k)) , where
 *            b(k) = 4 if k≡1,3,5,7,9 (mod 10), b(k) = 1 if k≡2,4,6,8 (mod
 *            10), b(k) = 0 if k≡0 (mod 10)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 50, u64 n <= 166, u128 n <= 590,
 *            big any n
 * Helpers:   mpb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mpb.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate zero‑initialized array of size n+1 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return (seq_t)0;         /* allocation failure */

    dp[0] = (seq_t)1;

    /* Main DP: iterate over part sizes k */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int m = mpb(k);
        if (m == 0u) continue;        /* skip multiples of 10 */

        /* Apply m copies of the ordinary partition factor (1 – x^k)^(-1) */
        for (unsigned int iter = 0; iter < m; ++iter) {
            for (idx_t j = k; j <= n; ++j) {
                dp[j] += dp[j - k];
            }
        }
    }

    seq_t res = dp[n];
    free(dp);
    return res;
}
