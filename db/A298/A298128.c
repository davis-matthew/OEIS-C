/*
 * A298128  Number of nX3 0..1 arrays with every element equal to 2, 3, 4 or 7
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over rows, enforcing that for each cell
 * in the n×3 binary array (upper‑left cell fixed to 0) the number of
 * neighboring cells (king moves) with the same value equals 2, 3, 4, or 7. */

#include <stdbool.h>

/* Check whether a single row (represented by currMask) satisfies the
 * adjacency rule, given the masks of the previous and next rows (if they exist). */
static bool row_valid(int prevMask, int currMask, int nextMask,
                      bool prevExists, bool nextExists)
{
    for (int col = 0; col < 3; ++col) {
        int curBit = (currMask >> col) & 1;
        int same = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == -1 && !prevExists) continue;
            if (dx == 1  && !nextExists)  continue;
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int ncol = col + dy;
                if (ncol < 0 || ncol >= 3) continue;
                int bit;
                if (dx == -1)
                    bit = (prevMask >> ncol) & 1;
                else if (dx == 0)
                    bit = (currMask >> ncol) & 1;
                else
                    bit = (nextMask >> ncol) & 1;
                if (bit == curBit) ++same;
            }
        }
        if (!(same == 2 || same == 3 || same == 4 || same == 7))
            return false;
    }
    return true;
}

/* OEIS A298128: number of n×3 binary arrays with upper‑left cell zero
 * such that each cell has 2, 3, 4 or 7 neighboring cells with the same value. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    if (n == 1) {
        seq_t sum = 0;
        for (int m = 0; m < 8; ++m) {
            if ((m & 1) == 0) {
                if (row_valid(0, m, 0, false, false))
                    sum += 1;
            }
        }
        return sum;
    }

    seq_t dp[8][8];
    seq_t next[8][8];

    /* initialize dp with first row (upper‑left element must be 0) */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            dp[i][j] = 0;
    for (int m0 = 0; m0 < 8; ++m0)
        if ((m0 & 1) == 0)
            dp[0][m0] = 1;

    /* process rows 1 .. n-1 */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = 0;

        bool prevExists = (r - 1 > 0);
        for (int p = 0; p < 8; ++p) {
            for (int c = 0; c < 8; ++c) {
                seq_t val = dp[p][c];
                if (val == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (row_valid(p, c, nxt, prevExists, true))
                        next[c][nxt] += val;
                }
            }
        }

        /* copy next to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* final check for the last row (no row below) */
    seq_t total = 0;
    for (int p = 0; p < 8; ++p) {
        for (int c = 0; c < 8; ++c) {
            seq_t val = dp[p][c];
            if (val == 0) continue;
            if (row_valid(p, c, 0, true, false))
                total += val;
        }
    }
    return total;
}
