/*
 * A302419  Number of 6 X n 0..1 arrays with every element equal to 0, 1, 2 or
 *          5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   is_valid_middle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of 6×n binary arrays whose
 * upper‑left element is 0 and where for every cell the
 * number of horizontally or diagonally adjacent cells
 * that have the same value is 0, 1, 2 or 5 (i.e. 3,4,6 are forbidden).
 * The algorithm uses a transfer matrix that remembers two
 * consecutive columns.  Only O(n·2³) time and O(1) memory are needed.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_valid_middle.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* constants */
    enum { ROWS = 6, STATES = 1 << ROWS, SENTINEL = STATES };   /* 0..63 valid, 64 = sentinel */

    /* static tables built once */
    static bool init = false;

    static uint8_t trans_out[SENTINEL + 1][STATES][STATES];
    static unsigned trans_len[SENTINEL + 1][STATES];
    static bool end_valid[SENTINEL + 1][STATES];

    if (!init) {
        for (int left = 0; left <= SENTINEL; ++left) {
            bool left_present = (left != SENTINEL);
            uint8_t Lbits = left_present ? (uint8_t)left : 0;
            for (int mid = 0; mid < STATES; ++mid) {
                trans_len[left][mid] = 0;
                for (int right = 0; right < STATES; ++right) {
                    if (is_valid_middle(Lbits, left_present,
                                        (uint8_t)mid, (uint8_t)right, true)) {
                        trans_out[left][mid][trans_len[left][mid]++] = (uint8_t)right;
                    }
                }
                end_valid[left][mid] = is_valid_middle(Lbits, left_present,
                                                       (uint8_t)mid, 0, false);
            }
        }
        init = true;
    }

    /* DP arrays */
    static seq_t dp[2][SENTINEL + 1][STATES];
    /* clear dp */
    for (int t = 0; t < 2; ++t)
        for (int l = 0; l <= SENTINEL; ++l)
            for (int m = 0; m < STATES; ++m)
                dp[t][l][m] = 0;

    int cur = 0, nxt = 1;
    /* initial column: top‑left bit (bit 0) must be 0 */
    for (int c = 0; c < STATES; ++c) {
        if ((c & 1u) == 0)
            dp[cur][SENTINEL][c] = 1;
    }

    /* iterate over remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next layer */
        for (int l = 0; l <= SENTINEL; ++l)
            for (int m = 0; m < STATES; ++m)
                dp[nxt][l][m] = 0;

        for (int left = 0; left <= SENTINEL; ++left) {
            for (int mid = 0; mid < STATES; ++mid) {
                seq_t val = dp[cur][left][mid];
                if (val == 0) continue;
                for (unsigned k = 0; k < trans_len[left][mid]; ++k) {
                    int right = trans_out[left][mid][k];
                    dp[nxt][mid][right] += val;
                }
            }
        }
        /* swap buffers */
        cur ^= 1;
        nxt ^= 1;
    }

    /* final column must be valid with left neighbour and no right neighbour */
    seq_t result = 0;
    for (int left = 0; left <= SENTINEL; ++left) {
        for (int curcol = 0; curcol < STATES; ++curcol) {
            if (end_valid[left][curcol])
                result += dp[cur][left][curcol];
        }
    }

    return result;
}
