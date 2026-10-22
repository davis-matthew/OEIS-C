/*
 * A349817  Number of 4 X n Fibonacci minimal checkerboards.
 * https://oeis.org/A349817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{(s1,s2,s3,s4)∈{0,1}^{n-1}^4 : each si has no adjacent
 *            1s and for all columns k, s1[k]∨s2[k]∨s3[k]∨s4[k] = 1}|
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over 4‑bit column states. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t N = n - (idx_t)1;          /* number of columns where dominoes may start */

    seq_t dp[16]  = {0};
    seq_t next[16];

    dp[0] = 1;                       /* start with no previous domino */

    for (idx_t i = 0; i < N; ++i) {
        /* clear next array */
        for (int s = 0; s < 16; ++s) next[s] = 0;

        for (int p = 0; p < 16; ++p) {
            seq_t val = dp[p];
            if (!val) continue;
            for (int q = 1; q < 16; ++q) {      /* q != 0 → at least one domino crosses the cut */
                if ((p & q) == 0) {
                    next[q] += val;
                }
            }
        }
        /* copy next to dp for next iteration */
        for (int s = 0; s < 16; ++s) dp[s] = next[s];
    }

    /* sum all final states */
    seq_t sum = 0;
    for (int s = 0; s < 16; ++s) sum += dp[s];
    return sum;
}
