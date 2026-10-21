/*
 * A265836  Expansion of Product_{k>=1} 1/(1 - k*(k+1)*x^k).
 * https://oeis.org/A265836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in
 *       ∏_{k≥1} 1 / (1 – k·(k+1)·x^k)
 *
 * The implementation uses a classic 1‑dimensional DP.
 * For each part size k the factor (1 – k(k+1)x^k)^(-1)
 * contributes a multiplicative weight w_k = k(k+1).
 *
 * DP recurrence:
 *   dp[0] = 1
 *   for k = 1 .. n
 *       for j = k .. n
 *           dp[j] += w_k * dp[j – k]
 *   a(n) = dp[n]
 *
 * The routine recomputes the table for every call, which is
 * simple, robust, and fast enough for n ≤ 10 000 (≈ 50 M
 * integer operations per call).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static DP buffer (reused across calls) */
    static seq_t *dp = NULL;
    static idx_t cap = 0;      /* allocated length of dp */

    /* grow the buffer if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *newdp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!newdp) abort();      /* allocation failure */

        free(dp);                  /* discard old contents */
        dp = newdp;
        cap = new_cap;
    }

    /* initialise dp[0..n] to zero */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;

    dp[0] = (seq_t)1;                      /* base case */

    /* main DP loops */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t w = (seq_t)k * ((seq_t)k + 1);   /* weight k(k+1) */
        for (idx_t j = k; j <= n; ++j) {
            dp[j] += w * dp[j - k];
        }
    }

    return dp[n];
}
