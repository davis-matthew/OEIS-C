/*
 * A253794  Number of (n+2)X(1+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum plus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+2) x 3 0..1 arrays in which the value
 *   f = (diagonal maximum of each 3x3 subblock) + (antidiagonal median)
 *   is nondecreasing from top to bottom across all 3x3 subblocks.
 *
 * This is computed by dynamic programming over the rows.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    size_t rows = n + 2;          // total number of rows
    seq_t dp[64][4]  = {0};        // dp[encoded_pair][prev_f]  (prev_f = 0,1,2,3)
    seq_t next[64][4] = {0};

    /* initialise with all possible first two rows; no block processed yet */
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int r2 = 0; r2 < 8; ++r2) {
            dp[(r1 << 3) | r2][3] = 1;   // sentinel value 3 → “no block processed”
        }
    }

    /* iterate over positions where a new row is added (row 3 … rows) */
    for (size_t pos = 3; pos <= rows; ++pos) {

        /* clear next table */
        for (int i = 0; i < 64; ++i)
            for (int f = 0; f < 4; ++f)
                next[i][f] = 0;

        /* transition */
        for (int enc = 0; enc < 64; ++enc) {
            int r1 = enc >> 3;
            int r2 = enc & 7;
            for (int prevf = 0; prevf < 4; ++prevf) {
                seq_t cur = dp[enc][prevf];
                if (cur == 0) continue;
                for (int r3 = 0; r3 < 8; ++r3) {

                    /* compute f for the 3×3 block (rows r1,r2,r3) */
                    int d0 = (r1 >> 0) & 1;   // top‑left
                    int d1 = (r2 >> 1) & 1;   // middle
                    int d2 = (r3 >> 2) & 1;   // bottom‑right
                    int diagmax = d0 | d1 | d2;      // maximum on main diagonal

                    int a0 = (r1 >> 2) & 1;   // top‑right
                    int a1 = (r2 >> 1) & 1;   // middle
                    int a2 = (r3 >> 0) & 1;   // bottom‑left
                    int antimedian = ((a0 + a1 + a2) >= 2) ? 1 : 0;

                    int fblock = diagmax + antimedian;

                    /* enforce monotonicity unless this is the first block */
                    if (prevf == 3 || fblock >= prevf) {
                        int newenc = (r2 << 3) | r3;
                        next[newenc][fblock] += cur;
                    }
                }
            }
        }

        /* move to the next step */
        for (int i = 0; i < 64; ++i)
            for (int f = 0; f < 4; ++f)
                dp[i][f] = next[i][f];
    }

    /* sum all final states (prev_f can only be 0,1,2 at this point) */
    seq_t total = 0;
    for (int i = 0; i < 64; ++i)
        for (int f = 0; f < 3; ++f)
            total += dp[i][f];
    return total;
}
