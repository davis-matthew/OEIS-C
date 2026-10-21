/*
 * A258505  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the diagonal and antidiagonal minus the sum of
 *          the medians of the central row and column nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A258505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts 0/1 (n+2)×4 matrices whose 3×3 window values (difference
 *            of two diagonal medians minus two row/column medians) are
 *            nondecreasing horizontally, vertically, and along the NE–SW
 *            antidiagonal
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 49,
 *            big any n
 * Helpers:   compute_window_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_window_value.h"

seq_t a(idx_t n)
{
    /* Precomputation of window values and horizontal feasibility. */
    static int win_left[16][16][16];
    static int win_right[16][16][16];
    static bool horiz_ok[16][16][16];
    static bool precomputed = false;
    if (!precomputed) {
        for (int p0 = 0; p0 < 16; ++p0)
            for (int p1 = 0; p1 < 16; ++p1)
                for (int p2 = 0; p2 < 16; ++p2) {
                    int l = compute_window_value(p0, p1, p2, 0);
                    int r = compute_window_value(p0, p1, p2, 1);
                    win_left[p0][p1][p2]  = l + 2; /* map [-2,2] → [0,4] */
                    win_right[p0][p1][p2] = r + 2;
                    horiz_ok[p0][p1][p2]  = (l <= r);
                }
        precomputed = true;
    }

    /* DP tables: dp[cur][row0][row1][leftIdx][rightIdx] */
    static seq_t dp[2][16][16][5][5];
    int cur = 0, nxt = 1;

    /* Initialize all entries to zero. */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 5; ++c)
                for (int d = 0; d < 5; ++d)
                    dp[cur][a][b][c][d] = 0;

    /* Base states: two initial rows, no window computed yet (sentinel index 4). */
    for (int p0 = 0; p0 < 16; ++p0)
        for (int p1 = 0; p1 < 16; ++p1)
            dp[cur][p0][p1][4][4] = (seq_t)1;

    /* Number of rows to add: from index 2 to n+1 inclusive. */
    for (int step = 2; step <= (int)n + 1; ++step) {

        /* Zero next table. */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 5; ++c)
                    for (int d = 0; d < 5; ++d)
                        dp[nxt][a][b][c][d] = 0;

        /* Iterate over current states. */
        for (int r0 = 0; r0 < 16; ++r0)
            for (int r1 = 0; r1 < 16; ++r1)
                for (int li = 0; li < 5; ++li)
                    for (int ri = 0; ri < 5; ++ri) {
                        seq_t cnt = dp[cur][r0][r1][li][ri];
                        if (cnt == 0) continue;

                        /* Try all possible next rows. */
                        for (int r2 = 0; r2 < 16; ++r2) {
                            if (!horiz_ok[r0][r1][r2]) continue;

                            int new_li = win_left[r0][r1][r2];
                            int new_ri = win_right[r0][r1][r2];
                            int val_l = new_li - 2;
                            int val_r = new_ri - 2;

                            /* Vertical and cross constraints apply only after first window. */
                            bool ok = true;
                            if (li != 4) {   /* previous window exists */
                                int prev_l = li - 2;
                                int prev_r = ri - 2;
                                if (prev_l > val_l || prev_r > val_r || prev_r > val_l)
                                    ok = false;
                            }

                            if (!ok) continue;

                            /* Update next DP state. */
                            dp[nxt][r1][r2][new_li][new_ri] += cnt;
                        }
                    }

        /* Swap tables. */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all counts in the final DP table. */
    seq_t result = 0;
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int li = 0; li < 5; ++li)
                for (int ri = 0; ri < 5; ++ri)
                    result += dp[cur][r0][r1][li][ri];

    return result;
}
