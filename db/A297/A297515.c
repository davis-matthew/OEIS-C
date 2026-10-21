/*
 * A297515  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays where every 1 is *
 *            horizontally, diagonally or antidiagonally adjacent to exactly *
 *            1 or 3 neighboring 1s. (OEIS A297515) * * The algorithm uses
 *            dynamic programming over pairs of consecutive * rows. For a
 *            transition from two previous rows `pp` and `p` to a * new r...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   good_last, trans_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/good_last.h"
#include "../utils/trans_ok.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Handle the trivial first row case separately. */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t p = 0; p < 16; ++p) {
            if (good_last(0, p))
                ans += 1;
        }
        return ans;
    }

    /* DP tables: dp[prev][curr] counts arrangements up to the
     * current row, where prev is the pattern of the previous row
     * and curr is the pattern of the current row. */
    seq_t dp[16][16] = {0};
    seq_t ndp[16][16];
    /* Initial state: before the first row, pretend prevprev = 0. */
    for (uint8_t p = 0; p < 16; ++p)
        dp[0][p] = 1;

    /* Process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero new DP */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        /* Transitions */
        for (int pp = 0; pp < 16; ++pp) {
            for (int p = 0; p < 16; ++p) {
                seq_t val = dp[pp][p];
                if (!val) continue;
                for (int m = 0; m < 16; ++m) {
                    if (!trans_ok(pp, p, m))
                        continue;
                    ndp[p][m] += val;
                }
            }
        }
        /* Move to next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over final states where the last row is valid */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (good_last(prev, curr))
                ans += dp[prev][curr];
        }
    }
    return ans;
}
