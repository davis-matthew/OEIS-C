/*
 * A251795  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over 64 states representing rows of length 3 with entries
 *            0..3, counting sequences of length n+1 that satisfy the 2×2
 *            subblock inequality (x11 + x01 ≥ x10 + x00 and x22 + x12 ≥ x21 +
 *            x11). The count is obtained by repeatedly multiplying a 64×64
 *            transfer matrix, yielding the total number of adm...
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* for safety – offset starts at 1 */
        return 0;

    /* Precompute the 64×64 transfer matrix only once */
    static bool allowed[64][64];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 64; ++i) {
            int p[3];
            p[0] = i & 3;
            p[1] = (i >> 2) & 3;
            p[2] = (i >> 4) & 3;
            for (int j = 0; j < 64; ++j) {
                int q[3];
                q[0] = j & 3;
                q[1] = (j >> 2) & 3;
                q[2] = (j >> 4) & 3;
                if (q[1] + p[1] >= q[0] + p[0] && q[2] + p[2] >= q[1] + p[1])
                    allowed[i][j] = true;
                else
                    allowed[i][j] = false;
            }
        }
        init = true;
    }

    seq_t dp[64], newdp[64];
    for (int i = 0; i < 64; ++i)
        dp[i] = 1;          /* one way to have the first row be any state */

    /* Each iteration extends the array by one more row */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            newdp[i] = 0;
        for (int prev = 0; prev < 64; ++prev) {
            seq_t val = dp[prev];
            if (val == 0) continue;
            for (int next = 0; next < 64; ++next) {
                if (allowed[prev][next])
                    newdp[next] += val;
            }
        }
        for (int i = 0; i < 64; ++i)
            dp[i] = newdp[i];
    }

    /* Sum all possible ending rows to get the total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        total += dp[i];
    return total;
}
