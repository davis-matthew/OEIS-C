/*
 * A338491  Least number of centered pentagonal numbers needed to represent n.
 * https://oeis.org/A338491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of centered pentagonal numbers (numbers of
 *            the form 5k(k-1)/2 + 1, k ≥ 1) whose sum equals n. The algorithm
 *            uses classic unbounded coin‑change DP
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache covering all n previously computed.  dp[0] = 0,
       dp[i] = minimal number of centered pentagonal numbers summing
       to i. */
    static seq_t *dp = NULL;
    static idx_t   dp_cap = 0;   /* current allocated size (dp_cap-1 is max index) */

    /* Recompute DP only when a larger n is requested. */
    if (dp == NULL || n >= dp_cap) {
        idx_t new_cap = n + 1;                 /* indices 0 .. n inclusive */
        seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_dp) abort();                  /* OOM protection */

        new_dp[0] = (seq_t)0;
        for (idx_t i = 1; i < new_cap; ++i)
            new_dp[i] = (seq_t)new_cap;        /* large initial value */

        /* Scan all centered pentagonal numbers ≤ n and update DP. */
        for (idx_t k = 1;; ++k) {
            seq_t coin = (seq_t)5 * (seq_t)k * (seq_t)(k - 1) / 2 + (seq_t)1;
            if (coin > n) break;

            idx_t coin_idx = (idx_t)coin;
            for (idx_t s = coin_idx; s < new_cap; ++s) {
                seq_t cand = new_dp[s - coin_idx] + (seq_t)1;
                if (cand < new_dp[s])
                    new_dp[s] = cand;
            }
        }

        if (dp) free(dp);
        dp    = new_dp;
        dp_cap = new_cap;
    }

    return dp[n];
}
