/*
 * A282965  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A282965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with exactly two cells whose
 *            1-valued neighbors exceed one
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a dynamic programming over rows using a 16×16×3 state space. */
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not defined for n < 1 */

    /* Pre‑compute the extra violations added when a row (prevrow)
       is finalized given the previous two rows.                     */
    static unsigned char viol_table[16][16][16];
    static int table_ready = 0;
    if (!table_ready) {
        for (int x = 0; x < 16; ++x)
            for (int y = 0; y < 16; ++y)
                for (int z = 0; z < 16; ++z) {
                    unsigned char cnt = 0;
                    for (int j = 0; j < 4; ++j) {
                        if (!(y >> j & 1)) continue;          /* cell is 0 */
                        unsigned deg = 0;
                        if (j > 0 && (y >> (j - 1) & 1)) deg++;
                        if (j < 3 && (y >> (j + 1) & 1)) deg++;
                        if (x >> j & 1)                      deg++;
                        if (j < 3 && (x >> (j + 1) & 1))     deg++;
                        if (z >> j & 1)                      deg++;
                        if (j > 0 && (z >> (j - 1) & 1))     deg++;
                        if (deg > 1) cnt++;
                    }
                    viol_table[x][y][z] = cnt;
                }
        table_ready = 1;
    }

    /* DP state: dp[prevprev][prevrow][violations so far] */
    seq_t dp[16][16][3];
    seq_t ndp[16][16][3];
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;
    dp[0][0][0] = 1;

    /* Process each row */
    for (idx_t i = 0; i < n; ++i) {
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 3; ++c)
                    ndp[a][b][c] = 0;

        for (int prevprev = 0; prevprev < 16; ++prevprev) {
            for (int prevrow = 0; prevrow < 16; ++prevrow) {
                for (int viol = 0; viol < 3; ++viol) {
                    seq_t curcnt = dp[prevprev][prevrow][viol];
                    if (curcnt == 0) continue;
                    for (int cur = 0; cur < 16; ++cur) {
                        unsigned char extra = viol_table[prevprev][prevrow][cur];
                        unsigned newviol = viol + extra;
                        if (newviol > 2) continue;
                        ndp[prevrow][cur][newviol] += curcnt;
                    }
                }
            }
        }
        /* copy ndp to dp */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                for (int c = 0; c < 3; ++c)
                    dp[a][b][c] = ndp[a][b][c];
    }

    /* Finalize the last row using a non-existing below row (pattern 0) */
    seq_t result = 0;
    for (int prevprev = 0; prevprev < 16; ++prevprev) {
        for (int prevrow = 0; prevrow < 16; ++prevrow) {
            for (int viol = 0; viol < 3; ++viol) {
                seq_t curcnt = dp[prevprev][prevrow][viol];
                if (curcnt == 0) continue;
                unsigned char extra = viol_table[prevprev][prevrow][0];
                if (viol + extra == 2)
                    result += curcnt;
            }
        }
    }
    return result;
}
