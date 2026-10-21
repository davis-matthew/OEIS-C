/*
 * A257183  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the diagonal and antidiagonal minus the sum of
 *          the minimums of the central row and column nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A257183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary arrays such that the sequence of
 *            values b(i,j) = median(diag)+median(anti-diag) – (min central
 *            row + min central column) for each 3×3 subblock (i=0..n-1,
 *            j=0,1) is nondecreasing when read left→right, top→bottom and
 *            along the NE→SW antidiagonal
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 24, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0/1 row pattern of length 4  (bits 0..3: columns 0..3)   */
#include <stdint.h>
#include <stdbool.h>

static const int PAT[16] = {
    0x0, 0x1, 0x3, 0x7, 0xF, 0x9, 0xD, 0xB, 0x5, 0x6, 0xA, 0xC, 0x2, 0x8, 0x4, 0xE
};
        /* PAT[0] = 0000     PAT[1] = 0001
           PAT[2] = 0011     PAT[3] = 0111
           PAT[4] = 1111     ...   */

static int8_t diff0[16][16][16];
static int8_t diff1[16][16][16];

/* Precompute the block values for every triple of rows. */
static void init_diff(void) {
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            for (int r2 = 0; r2 < 16; ++r2) {
                for (int j = 0; j < 2; ++j) {
                    int x0 = (r0 >> j) & 1;
                    int x1 = (r0 >> (j + 1)) & 1;
                    int x2 = (r0 >> (j + 2)) & 1;

                    int y0 = (r1 >> j) & 1;
                    int y1 = (r1 >> (j + 1)) & 1;
                    int y2 = (r1 >> (j + 2)) & 1;

                    int z0 = (r2 >> j) & 1;
                    int z1 = (r2 >> (j + 1)) & 1;
                    int z2 = (r2 >> (j + 2)) & 1;

                    int med_diag = (x0 + y1 + z2) >= 2;
                    int med_anti = (x2 + y1 + z0) >= 2;
                    int min_row  = y0 & y1 & y2;
                    int min_col  = x1 & y1 & z1;

                    int val = med_diag + med_anti - min_row - min_col;
                    if (j == 0)
                        diff0[r0][r1][r2] = (int8_t)val;
                    else
                        diff1[r0][r1][r2] = (int8_t)val;
                }
            }
}

/* Main counting function. */
seq_t a(idx_t n) {
    const idx_t rows = n + 2;            /* total number of rows */

    if (n == 0) return 0;               /* not defined for offset 0 */

    /* Initialise diff tables once */
    static bool diff_init = false;
    if (!diff_init) {
        init_diff();
        diff_init = true;
    }

    /* DP tables: [prev first][prev second][v0+2][v1+2] */
    static seq_t dp[16][16][5][5];
    static seq_t ndp[16][16][5][5];

    /* Clear dp */
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 5; ++c)
                for (int d = 0; d < 5; ++d)
                    dp[a][b][c][d] = 0;

    /* Initialisation: first three row patterns */
    for (int p0 = 0; p0 < 16; ++p0)
        for (int p1 = 0; p1 < 16; ++p1)
            for (int p2 = 0; p2 < 16; ++p2) {
                int8_t v0 = diff0[p0][p1][p2];
                int8_t v1 = diff1[p0][p1][p2];
                if (v1 < v0) continue;                  /* horizontal monotone */
                dp[p1][p2][v0 + 2][v1 + 2] += (seq_t)1;
            }

    /* Process the remaining rows (index 3 .. rows-1) */
    for (idx_t r = 3; r < rows; ++r) {
        /* clear ndp */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 5; ++c)
                    for (int d = 0; d < 5; ++d)
                        ndp[a][b][c][d] = 0;

        for (int pp0 = 0; pp0 < 16; ++pp0)
            for (int pp1 = 0; pp1 < 16; ++pp1)
                for (int v0 = 0; v0 < 5; ++v0)
                    for (int v1 = 0; v1 < 5; ++v1) {
                        seq_t cur = dp[pp0][pp1][v0][v1];
                        if (!cur) continue;
                        int prev_val0 = v0 - 2;
                        int prev_val1 = v1 - 2;
                        for (int pnew = 0; pnew < 16; ++pnew) {
                            /* vertical monotonicity: previous second row <= new row */
                            /* No explicit check needed as all rows are allowed,
                               the diff constraints handle ordering. */

                            /* Compute current block values for new triple (pp0,pp1,pnew) */
                            int8_t cur_v0 = diff0[pp0][pp1][pnew];
                            int8_t cur_v1 = diff1[pp0][pp1][pnew];
                            if (cur_v1 < cur_v0) continue;              /* horizontal */
                            if (cur_v0 < prev_val0) continue;           /* vertical on offset 0 */
                            if (cur_v1 < prev_val1) continue;           /* vertical on offset 1 */
                            if (cur_v0 < prev_val1) continue;           /* antidiagonal */

                            ndp[pp1][pnew][cur_v0 + 2][cur_v1 + 2] += cur;
                        }
                    }

        /* move ndp -> dp */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 5; ++c)
                    for (int d = 0; d < 5; ++d)
                        dp[a][b][c][d] = ndp[a][b][c][d];
    }

    /* Sum all final states */
    seq_t total = 0;
    for (int a = 0; a < 16; ++a)
        for (int b = 0; b < 16; ++b)
            for (int c = 0; c < 5; ++c)
                for (int d = 0; d < 5; ++d)
                    total += dp[a][b][c][d];

    return total;
}
