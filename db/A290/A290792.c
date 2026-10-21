/*
 * A290792  Expansion of Product_{k>=1} 1/(1 - x^(k*(k+1)^2*(k+2)/12)).
 * https://oeis.org/A290792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 789, u32 n <= 5303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partitions into 4‑dimensional pyramidal numbers (OEIS A290792) */
/* The recurrence is the classic partition DP using allowed parts
   p(k) = k*(k+1)^2*(k+2)/12 (k >= 1). */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* Expand or initialize cache if necessary */
    if (!dp || n > max_n) {
        idx_t new_n = n;

        /* Allocate new DP array and zero it */
        seq_t *new_dp = (seq_t*)malloc((new_n + 1) * sizeof(seq_t));
        if (!new_dp) abort();
        for (idx_t i = 0; i <= new_n; ++i)
            new_dp[i] = 0;

        new_dp[0] = 1;  /* Base case: one way to partition 0 */

        /* Generate all pyramidal parts <= new_n and run DP */
        for (idx_t k = 1;; ++k) {
            idx_t kk = k;
            idx_t part = kk * (kk + 1) * (kk + 1) * (kk + 2) / 12;
            if (part > new_n) break;
            for (idx_t i = part; i <= new_n; ++i) {
                new_dp[i] = new_dp[i] + new_dp[i - part];
            }
        }

        free(dp);
        dp = new_dp;
        max_n = new_n;
    }

    return dp[n];
}
