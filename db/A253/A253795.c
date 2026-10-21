/*
 * A253795  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum plus antidiagonal median nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   this code computes a(n) of OEIS A253795 by dynamic programming
 *            over the rows of the (n+2)×4 0–1 array. It does not use a closed
 *            form but enumerates all feasible 3×3 subblocks and propagates
 *            nondecreasing constraints horizontally and vertically
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* 3×3 subblock derivative values:
   ff = diagonal maximum (∈{0,1,2})  +  antidiagonal median (∈{0,1})
   The resulting value ff∈{0,1,2,3,4,5} is encoded as a pair
   (ff0,ff1) where ff0=diagonal, ff1=antidiagonal.
   The DP enforces nondecreasing ff0 and ff1 both horizontally
   (between adjacent columns) and vertically (between adjacent
   windows of rows).  The remaining details are in the code. */

seq_t a(idx_t n)
{
    /* Pre‑compute for all 3‑row combinations the values of a
       3×3 subblock starting at columns 1–3 and 2–4, and whether
       the horizontal monotonicity holds inside that window. */
    static uint8_t f0[16][16][16];
    static uint8_t f1[16][16][16];
    static bool   ok[16][16][16];
    static bool   init = false;

    if (!init) {
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2)
                for (int r3 = 0; r3 < 16; ++r3) {
                    /* Window columns 1–3 */
                    uint8_t d0 = ((r1>>0)&1) | ((r2>>1)&1) | ((r3>>2)&1);
                    uint8_t a0 = ((r1>>2)&1) + ((r2>>1)&1) + ((r3>>0)&1);
                    uint8_t m0 = (a0 >= 2) ? 1 : 0;
                    uint8_t ff0 = d0 + m0;        /* 0..2 */

                    /* Window columns 2–4 */
                    uint8_t d1 = ((r1>>1)&1) | ((r2>>2)&1) | ((r3>>3)&1);
                    uint8_t a1 = ((r1>>3)&1) + ((r2>>2)&1) + ((r3>>1)&1);
                    uint8_t m1 = (a1 >= 2) ? 1 : 0;
                    uint8_t ff1 = d1 + m1;        /* 0..2 */

                    f0[r1][r2][r3] = ff0;
                    f1[r1][r2][r3] = ff1;
                    ok[r1][r2][r3] = (ff0 <= ff1);      /* horizontal rule inside the window */
                }
        init = true;
    }

    /* DP tables: static to avoid stack overflow for non‑POD seq_t. */
    static seq_t dpPrev[16][16][9];
    static seq_t dpNext[16][16][9];

    /* Zero initialise the DP tables for this call. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 9; ++k) {
                dpPrev[i][j][k] = (seq_t)0;
                dpNext[i][j][k] = (seq_t)0;
            }

    /* Initialise DP with the first 3×3 window (rows 1–3). */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2)
            for (int r3 = 0; r3 < 16; ++r3) {
                if (!ok[r1][r2][r3]) continue;
                uint8_t ff0 = f0[r1][r2][r3];
                uint8_t ff1 = f1[r1][r2][r3];
                uint8_t idx = ff0 * 3 + ff1;          /* 0..8 */
                dpPrev[r2][r3][idx] += (seq_t)1;
            }

    /* Process remaining windows vertically. */
    for (idx_t step = 2; step <= n; ++step) {
        /* Zero dpNext */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 9; ++k)
                    dpNext[i][j][k] = (seq_t)0;

        for (int rPrev = 0; rPrev < 16; ++rPrev)
            for (int rCur  = 0; rCur  < 16; ++rCur)
                for (int fid   = 0; fid   < 9;  ++fid) {
                    seq_t cnt = dpPrev[rPrev][rCur][fid];
                    if (cnt == (seq_t)0) continue;
                    uint8_t prev_ff0 = fid / 3;
                    uint8_t prev_ff1 = fid % 3;
                    for (int newRow = 0; newRow < 16; ++newRow) {
                        if (!ok[rPrev][rCur][newRow]) continue;
                        uint8_t ff0 = f0[rPrev][rCur][newRow];
                        uint8_t ff1 = f1[rPrev][rCur][newRow];
                        if (prev_ff0 <= ff0 && prev_ff1 <= ff1) {
                            uint8_t newIdx = ff0 * 3 + ff1;
                            dpNext[rCur][newRow][newIdx] += cnt;
                        }
                    }
                }

        /* Copy dpNext into dpPrev for next iteration. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 9; ++k)
                    dpPrev[i][j][k] = dpNext[i][j][k];
    }

    /* Sum all states: each represents a valid (n+2)×4 array. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 9; ++k)
                result += dpPrev[i][j][k];

    return result;
}
