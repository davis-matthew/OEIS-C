/*
 * A299550  Number of nX4 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×4 binary arrays with every element having 2,
 *            3, 4, 5, 6 or 8 king-move adjacent elements, the upper left
 *            element zero. Computed by a row-based dynamic programming over
 *            the 4-column grid
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   allowed_count, valid_bottom, valid_only_row, valid_top
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses an O(n) time, O(1) memory algorithm. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/allowed_count.h"
#include "../utils/valid_bottom.h"
#include "../utils/valid_only_row.h"
#include "../utils/valid_top.h"

/* Precompute transition validity for internal rows (both neighbors exist). */
static bool trans[16][16][16];
static bool trans_precomputed = false;
static void precompute_trans(void) {
    if (trans_precomputed) return;
    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int cur = (b >> col) & 1;
                    int same = 0;

                    /* same row neighbors */
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dc == 0) continue;
                        int nc = col + dc;
                        if (nc < 0 || nc >= 4) continue;
                        int nb = (b >> nc) & 1;
                        if (nb == cur) ++same;
                    }

                    /* above row neighbors (a) */
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nc = col + dc;
                        if (nc < 0 || nc >= 4) continue;
                        int nb = (a >> nc) & 1;
                        if (nb == cur) ++same;
                    }

                    /* below row neighbors (c) */
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nc = col + dc;
                        if (nc < 0 || nc >= 4) continue;
                        int nb = (c >> nc) & 1;
                        if (nb == cur) ++same;
                    }

                    if (!allowed_count(same)) {
                        ok = false;
                        break;
                    }
                }
                trans[a][b][c] = ok;
            }
        }
    }
    trans_precomputed = true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined below, but safe */

    /* Special case n == 1 : only one row, no neighbors up or down */
    if (n == 1) {
        seq_t count = 0;
        for (int bits = 0; bits < 16; ++bits) {
            if ((bits & 1) != 0) continue; /* upper-left must be zero */
            if (valid_only_row(bits)) count += 1;
        }
        return count;
    }

    precompute_trans();

    /* DP arrays for states (prev_prev_row, prev_row) */
    seq_t dp[256] = {0};
    seq_t newdp[256] = {0};

    /* Initialize with rows 0 and 1 */
    for (int row0 = 0; row0 < 16; ++row0) {
        if ((row0 & 1) != 0) continue; /* upper-left zero */
        for (int row1 = 0; row1 < 16; ++row1) {
            if (valid_top(0, row0, row1)) {
                int idx = (row0 << 4) | row1;
                dp[idx] += 1;
            }
        }
    }

    /* Process subsequent rows 2 .. n-1 */
    for (idx_t r = 2; r < n; ++r) {
        for (int i = 0; i < 256; ++i) newdp[i] = 0; /* clear newdp */

        for (int idx = 0; idx < 256; ++idx) {
            seq_t ways = dp[idx];
            if (!ways) continue;
            int row_prevprev = idx >> 4;
            int row_prev = idx & 15;
            for (int new_row = 0; new_row < 16; ++new_row) {
                if (trans[row_prevprev][row_prev][new_row]) {
                    int new_idx = (row_prev << 4) | new_row;
                    newdp[new_idx] += ways;
                }
            }
        }
        /* swap dp and newdp */
        for (int i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* Final summation: validate last row (no lower neighbor) */
    seq_t result = 0;
    for (int idx = 0; idx < 256; ++idx) {
        seq_t ways = dp[idx];
        if (!ways) continue;
        int row_prev = idx >> 4;
        int row_last = idx & 15;
        if (valid_bottom(row_prev, row_last, 0)) {
            result += ways;
        }
    }
    return result;
}
