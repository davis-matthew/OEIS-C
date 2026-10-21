/*
 * A297452  Number of n X 3 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A297452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×3 0/1 arrays where each 1 has 0 or 2 *
 *            horizontally or diagonally adjacent 1s. Computed by DP over rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset below 1 is undefined */

    /* validity table for triples (row_above, row_middle, row_below) */
    static bool valid[8][8][8];
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        for (int a = 0; a < 8; ++a)          /* row_above   */
            for (int b = 0; b < 8; ++b)      /* row_middle  */
                for (int c = 0; c < 8; ++c) {/* row_below   */
                    bool ok = true;
                    for (int col = 0; col < 3 && ok; ++col) {
                        int cell = (b >> col) & 1;
                        if (!cell) continue;

                        int neigh = 0;
                        /* same row left/right */
                        if (col > 0) neigh += (b >> (col - 1)) & 1;
                        if (col < 2) neigh += (b >> (col + 1)) & 1;
                        /* diagonals from above row */
                        if (col > 0) neigh += (a >> (col - 1)) & 1;
                        if (col < 2) neigh += (a >> (col + 1)) & 1;
                        /* diagonals from below row */
                        if (col > 0) neigh += (c >> (col - 1)) & 1;
                        if (col < 2) neigh += (c >> (col + 1)) & 1;

                        if (neigh != 0 && neigh != 2) ok = false;
                    }
                    valid[a][b][c] = ok;
                }
    }

    seq_t dp[8][8] = {0}, next[8][8] = {0};

    /* start with two virtual zero rows */
    dp[0][0] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* reset next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = 0;

        /* transition */
        for (int prev2 = 0; prev2 < 8; ++prev2)
            for (int prev1 = 0; prev1 < 8; ++prev1) {
                seq_t cnt = dp[prev2][prev1];
                if (!cnt) continue;
                for (int cur = 0; cur < 8; ++cur)
                    if (valid[prev2][prev1][cur])
                        next[prev1][cur] += cnt;
            }

        /* copy next to dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* final validation with below zero row */
    seq_t result = 0;
    for (int prev2 = 0; prev2 < 8; ++prev2)
        for (int prev1 = 0; prev1 < 8; ++prev1) {
            seq_t cnt = dp[prev2][prev1];
            if (!cnt) continue;
            if (valid[prev2][prev1][0])
                result += cnt;
        }

    return result;
}
