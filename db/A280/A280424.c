/*
 * A280424  G.f.: Product_{k>=1} (1 + x^k) / (1 - x^(k*(k+1))).
 * https://oeis.org/A280424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A280424 (OEIS):  G.f. = Π_{k≥1} (1 + x^k) / (1 – x^(k(k+1)))
   The implementation uses dynamic programming:
   For each k we first add contributions from unlimited copies of weight
   m = k(k+1) (complete knapsack), then add the optional single part of
   weight k (0/1 knapsack).  The routine works for any seq_t/idx_t pair
   supplied by the build. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* DP array of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t*)malloc(sizeof(seq_t) * sz);
    if (!dp) return (seq_t)0;                    /* fallback */

    /* initialize */
    for (size_t i = 0; i < sz; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* main loops */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t m = (seq_t)k * ((seq_t)k + 1);      /* weight k(k+1) */
        idx_t ms = (idx_t)m;

        /* unlimited copies of weight m (complete knapsack) */
        if (ms <= n) {
            for (idx_t s = ms; s <= n; ++s) {
                dp[s] += dp[s - ms];
            }
        }

        /* optional single part of weight k (0/1 knapsack) */
        for (idx_t s = n; s >= k; --s) {
            dp[s] += dp[s - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
