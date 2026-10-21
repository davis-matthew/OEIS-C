/*
 * A298218  Number of nX5 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of n×5 binary arrays where each cell has
 * 0,1,2,4, or 5 same-valued king‑move neighbours.
 * Upper‑left cell fixed to 0.
 * Uses dynamic programming over rows represented as 5‑bit patterns.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Checks whether a middle row `cur` is valid when its adjacent
 * rows are `above` and `below`.  If a row is missing, the
 * corresponding flag is true and no neighbours from that row
 * are counted. */
static bool row_valid(int above, int cur, int below,
                      bool above_missing, bool below_missing)
{
    for (int col = 0; col < 5; ++col) {
        int val = (cur >> col) & 1;
        int cnt = 0;
        for (int dr = -1; dr <= 1; ++dr) {
            int r = 1 + dr;               /* 0: above, 1: cur, 2: below */
            if (r < 0 || r > 2) continue;
            if (r == 0 && above_missing) continue;
            if (r == 2 && below_missing) continue;

            int rowbits = (r == 0) ? above : (r == 1) ? cur : below;
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int c = col + dc;
                if (c < 0 || c > 4) continue;
                if (((rowbits >> c) & 1) == val) ++cnt;
            }
        }
        if (!(cnt == 0 || cnt == 1 || cnt == 2 || cnt == 4 || cnt == 5))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) is 16: all 5-bit rows starting with leading bit 0. */
    if (n == 1) return (seq_t)16;

    /* static transition tables – built once */
    static bool initialized = false;
    static signed char inner_next[32][32][32];
    static signed char top_next[32][32];
    static unsigned char inner_next_count[32][32];
    static unsigned char top_next_count[32];
    static bool bottom_valid[32][32];

    if (!initialized) {
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                unsigned char cnt = 0;
                for (int c = 0; c < 32; ++c) {
                    if (row_valid(a, b, c, false, false)) {
                        inner_next[a][b][cnt] = (signed char)c;
                        ++cnt;
                    }
                }
                inner_next_count[a][b] = cnt;
            }
        }
        for (int r0 = 0; r0 < 32; ++r0) {
            if (r0 & 1) continue;          /* upper-left must be 0 */
            unsigned char cnt = 0;
            for (int r1 = 0; r1 < 32; ++r1) {
                if (row_valid(0, r0, r1, true, false)) {
                    top_next[r0][cnt] = (signed char)r1;
                    ++cnt;
                }
            }
            top_next_count[r0] = cnt;
        }
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                bottom_valid[a][b] = row_valid(a, b, 0, false, true);
            }
        }
        initialized = true;
    }

    /* dp and newdp keep counts of ways for the last two rows */
    seq_t dp[32][32];
    seq_t newdp[32][32];

    /* zero initialise dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* initialise for the first two rows */
    for (int r0 = 0; r0 < 32; ++r0) {
        if (r0 & 1) continue;               /* upper-left = 0 */
        for (unsigned char k = 0; k < top_next_count[r0]; ++k) {
            int r1 = top_next[r0][k];
            dp[r0][r1] = (seq_t)1;
        }
    }

    /* n==2 case */
    if (n == 2) {
        seq_t result = 0;
        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                if (dp[a][b] && bottom_valid[a][b])
                    result += dp[a][b];
            }
        }
        return result;
    }

    /* Add rows 3..n */
    for (idx_t idx = 2; idx < n; ++idx) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t cnt = dp[a][b];
                if (!cnt) continue;
                unsigned char mc = inner_next_count[a][b];
                for (unsigned char k = 0; k < mc; ++k) {
                    int c = inner_next[a][b][k];
                    newdp[b][c] += cnt;
                }
            }
        }
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over valid bottom rows */
    seq_t result = 0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            if (dp[a][b] && bottom_valid[a][b])
                result += dp[a][b];
        }
    }
    return result;
}
