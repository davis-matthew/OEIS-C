/*
 * A264591  Let G[1](q) denote the g.f. for A003114 and G[2](q) the g.f. for
 *          A003106 (the two Rogers-Ramanujan identities). For i>=3, let
 *          G[i](q) = (G[i-1](q)-G[i-2](q))/q^(i-2). Sequence gives
 *          coefficients of G[4](q).
 * https://oeis.org/A264591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥0, k = n−m(m+3) ≥ 0} p_m(k) , where p_m(k) is the
 *            number of partitions of k with all parts ≤ m
 * Range:     u8 n <= 48, u16 n <= 117, u32 n <= 329, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    seq_t result = 0;

    for (idx_t m = 0; ; ++m) {
        /* shift = m*(m+3) */
        idx_t shift = m * (m + (idx_t)3);
        if (shift > n)
            break;

        idx_t k = n - shift;          /* k ≥ 0 */

        /* allocate array dp[0..k] of type seq_t, initialized to 0 */
        seq_t *dp = (seq_t *)malloc((k + 1) * sizeof(seq_t));
        if (!dp)
            return result;           /* out‑of‑memory fallback */

        memset(dp, 0, (k + 1) * sizeof(seq_t));
        dp[0] = 1;                    /* one way to partition 0 */

        /* dynamic programming: partitions of k with parts ≤ m */
        for (idx_t part = 1; part <= m; ++part) {
            for (idx_t i = part; i <= k; ++i) {
                dp[i] += dp[i - part];
            }
        }

        result += dp[k];

        free(dp);
    }

    return result;
}
