/*
 * A373012  Number of distinct partitions p of n such that max(p) == 1 mod 3.
 * https://oeis.org/A373012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with
 *            largest part ≡ 1 (mod 3)
 * Range:     u8 n <= 36, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for counts with largest part congruent 0,1,2 (mod 3). */
    seq_t *dp0 = (seq_t*)calloc(n + 1, sizeof(seq_t));
    seq_t *dp1 = (seq_t*)calloc(n + 1, sizeof(seq_t));
    seq_t *dp2 = (seq_t*)calloc(n + 1, sizeof(seq_t));
    if (!dp0 || !dp1 || !dp2) {
        /* In the unlikely event of allocation failure, return 0. */
        return (seq_t)0;
    }

    /* DP over parts t = 1..n (distinct parts). */
    for (idx_t t = 1; t <= n; ++t) {
        int mod = t % 3;
        for (idx_t s = n; s >= t; --s) {
            seq_t tot = (s == t) ? (seq_t)1
                                  : dp0[s - t] + dp1[s - t] + dp2[s - t];
            switch (mod) {
                case 0: dp0[s] += tot; break;
                case 1: dp1[s] += tot; break;
                case 2: dp2[s] += tot; break;
            }
        }
    }

    seq_t result = dp1[n];    /* partitions with largest part ≡ 1 (mod 3) */

    /* Free memory. */
    free(dp0);
    free(dp1);
    free(dp2);

    return result;
}
