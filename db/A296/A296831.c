/*
 * A296831  Number of nX5 0..1 arrays with each 1 adjacent to 0, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: 5×n binary arrays, 1’s must have 0, 3 or 5 king‑move 
   neighboring 1’s.  Dynamic programming with a 32×32 transfer
   matrix (two previous rows). */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether the middle row 'b' satisfies the neighbour
   condition given the rows above (a) and below (c). */
static bool row_is_valid(uint8_t a, uint8_t b, uint8_t c) {
    for (int j = 0; j < 5; ++j) {
        if ((b >> j) & 1U) {
            int cnt = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                uint8_t row;
                if (dx == -1)      row = a;
                else if (dx == 0)  row = b;
                else               row = c;
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;        /* skip self */
                    int col = j + dy;
                    if (col < 0 || col >= 5) continue;
                    if ((row >> col) & 1U) ++cnt;
                }
            }
            if (cnt != 0 && cnt != 3 && cnt != 5)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    /* Pre‑compute validity table once. */
    static bool comp[32][32][32];
    static bool comp_init = false;
    if (!comp_init) {
        comp_init = true;
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                for (int c = 0; c < 32; ++c)
                    comp[a][b][c] = row_is_valid((uint8_t)a, (uint8_t)b, (uint8_t)c);
    }

    /* DP arrays: dp[state] where state is pair (prev2, prev). */
    static seq_t dp[32][32];
    static seq_t nxt[32][32];
    seq_t (*cur)[32] = dp;
    seq_t (*next)[32] = nxt;

    /* Initialise dp: previous row = dummy zero, current row = any pattern. */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = 0;
    for (int pattern = 0; pattern < 32; ++pattern)
        cur[0][pattern] = (seq_t)1;

    /* Process rows 1 .. n-1. */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next[i][j] = 0;

        for (int a = 0; a < 32; ++a) {
            for (int b = 0; b < 32; ++b) {
                seq_t val = cur[a][b];
                if (val == (seq_t)0) continue;
                for (int c = 0; c < 32; ++c) {
                    if (comp[a][b][c]) {
                        next[b][c] = next[b][c] + val;
                    }
                }
            }
        }
        /* Swap current and next pointers. */
        seq_t (*tmp)[32] = cur;
        cur = next;
        next = tmp;
    }

    /* Add final dummy row (all zeros) to validate the last row. */
    seq_t result = (seq_t)0;
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            seq_t val = cur[a][b];
            if (val == (seq_t)0) continue;
            if (comp[a][b][0]) { /* dummy next row is zero */
                result = result + val;
            }
        }
    }
    return result;
}
