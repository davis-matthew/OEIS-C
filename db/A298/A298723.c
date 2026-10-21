/*
 * A298723  Number of nX4 0..1 arrays with every element equal to 0, 2, 3, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with upper‑left element 0
 *            such that for each cell the number of same‑valued king‑move
 *            neighbours belongs to {0,2,3,5,8}
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>

seq_t a(idx_t n)
{
    const unsigned int DUMMY = 16;                     /* placeholder for non‑existent row */
    /* allowed neighbour counts */
    static const unsigned char allowed[9] = {1,0,1,1,0,1,0,0,1};

    /* transition table: transition[prev][curr][next] = 1 if cur row satisfies
       all constraints when prev, curr, next are provided. Prev and next can
       be DUMMY. Size 17×16×17 (prev 0‑16, curr 0‑15, next 0‑16). */
    static unsigned char transition[17][16][17];
    static int init = 0;
    if (!init) {
        for (int p = 0; p < 17; ++p) {
            for (int c = 0; c < 16; ++c) {
                for (int nx = 0; nx < 17; ++nx) {
                    unsigned char ok = 1;
                    for (int col = 0; col < 4; ++col) {
                        int val = (c >> col) & 1;
                        int same = 0;
                        /* horizontal neighbours */
                        if (col > 0 && ((c >> (col-1)) & 1) == val) ++same;
                        if (col < 3 && ((c >> (col+1)) & 1) == val) ++same;
                        /* above neighbours (if present) */
                        if (p != DUMMY) {
                            if (col > 0 && ((p >> (col-1)) & 1) == val) ++same;
                            if (((p >> col) & 1) == val) ++same;
                            if (col < 3 && ((p >> (col+1)) & 1) == val) ++same;
                        }
                        /* below neighbours (if present) */
                        if (nx != DUMMY) {
                            if (col > 0 && ((nx >> (col-1)) & 1) == val) ++same;
                            if (((nx >> col) & 1) == val) ++same;
                            if (col < 3 && ((nx >> (col+1)) & 1) == val) ++same;
                        }
                        if (!allowed[same]) { ok = 0; break; }
                    }
                    transition[p][c][nx] = ok;
                }
            }
        }
        init = 1;
    }

    seq_t dp[17][16];
    seq_t newdp[17][16];
    /* initialise dp[prev][curr] for the first row (prev = DUMMY) */
    for (int i = 0; i < 17; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (int row = 0; row < 16; ++row) {
        /* upper‑left element (col 0) must be 0 */
        if ((row & 1) == 0)
            dp[DUMMY][row] = 1;
    }

    /* process remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        memset(newdp, 0, sizeof(newdp));
        for (int p = 0; p < 17; ++p) {
            for (int c = 0; c < 16; ++c) {
                seq_t cnt = dp[p][c];
                if (cnt == 0) continue;
                for (int nx = 0; nx < 16; ++nx) {
                    if (transition[p][c][nx])
                        newdp[c][nx] += cnt;
                }
            }
        }
        /* copy newdp back to dp */
        for (int i = 0; i < 17; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final row requires checking against dummy below row */
    seq_t result = 0;
    for (int p = 0; p < 17; ++p) {
        for (int c = 0; c < 16; ++c) {
            seq_t cnt = dp[p][c];
            if (cnt == 0) continue;
            if (transition[p][c][DUMMY])
                result += cnt;
        }
    }
    return result;
}
