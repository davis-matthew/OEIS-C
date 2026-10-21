/*
 * A278284  Number of 5 X n 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   check_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Implementation of OEIS sequence A278284.  
   Counts 5×n binary arrays whose upper‑left cell is 0 and which satisfy:
   Each cell has at least one equal neighbour and at least one different
   neighbour among the positions
   (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) (1,0).  */
#include <stdbool.h>
#include <string.h>
#include "../utils/check_column.h"

seq_t a(idx_t n)
{
    /* All 32 possible 5‑bit column patterns (0..31) */
    static const int pat[32] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9,10,11,12,13,14,15,
        16,17,18,19,20,21,22,23,
        24,25,26,27,28,29,30,31
    };

    /* Transition table for a middle column */
    static bool trans[32][32][32];
    static bool trans_done = false;
    if (!trans_done) {
        for (int a = 0; a < 32; ++a)
            for (int b = 0; b < 32; ++b)
                for (int c = 0; c < 32; ++c)
                    trans[a][b][c] = check_column(pat[a], pat[b], pat[c], true, true);
        trans_done = true;
    }

    /* Special case n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < 32; ++i) {
            if ((pat[i] & 1) != 0) continue;                 /* top-left must be 0 */
            if (check_column(0, pat[i], 0, false, false))
                ans += (seq_t)1;
        }
        return ans;
    }

    static seq_t dp_prev[32 * 32];
    static seq_t dp_cur [32 * 32];
    memset(dp_prev, 0, sizeof(dp_prev));

    /* Initialize (column 1, column 2) */
    for (int i = 0; i < 32; ++i) {
        if ((pat[i] & 1) != 0) continue;                     /* top-left 0 */
        for (int j = 0; j < 32; ++j) {
            if (check_column(0, pat[i], pat[j], false, true))
                dp_prev[i * 32 + j] = (seq_t)1;
        }
    }

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        memset(dp_cur, 0, sizeof(dp_cur));
        for (int s = 0; s < 32 * 32; ++s) {
            seq_t ways = dp_prev[s];
            if (!ways) continue;
            int a = s >> 5;          /* previous column pattern (0..31) */
            int b = s & 0x1F;        /* current column pattern   */
            for (int c = 0; c < 32; ++c) {
                if (trans[a][b][c]) {
                    dp_cur[b * 32 + c] += ways;
                }
            }
        }
        memcpy(dp_prev, dp_cur, sizeof(dp_cur));
    }

    /* Final step: check last column (no right neighbour) */
    seq_t ans = 0;
    for (int s = 0; s < 32 * 32; ++s) {
        seq_t ways = dp_prev[s];
        if (!ways) continue;
        int a = s >> 5;          /* the second‑last column pattern */
        int b = s & 0x1F;        /* the last column pattern      */
        if (check_column(pat[a], pat[b], 0, true, false))
            ans += ways;
    }
    return ans;
}
