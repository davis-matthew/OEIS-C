/*
 * A283198  Number of n X 3 0..1 arrays with no 1 equal to more than two of
 *          its horizontal and vertical neighbors, with the exception of
 *          exactly one element.
 * https://oeis.org/A283198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A283198
   Number of n×3 0..1 arrays with exactly one 1 that has more than
   two of its horizontal and vertical neighbors.
   This implementation uses a transfer‑matrix dynamic programming
   over triples of consecutive rows.  It works for any sequence type
   seq_t and index type idx_t. */

#include <stdint.h>
#include <string.h>

/* Precompute violations for a given row with given neighbours.
   viol[up][row][down] == number of 1‑cells in 'row' that
   have >2 neighbors considering horiz., up and down rows. */
static unsigned char viol[8][8][8];
static int viol_inited = 0;

static void init_viol(void)
{
    if (!viol_inited) {
        for (int up = 0; up < 8; ++up) {
            for (int row = 0; row < 8; ++row) {
                for (int down = 0; down < 8; ++down) {
                    int v = 0;
                    for (int col = 0; col < 3; ++col) {
                        if (!(row & (1 << col))) continue;        /* not a 1‐cell */
                        int cnt = 0;
                        /* left neighbour */
                        if (col > 0 && (row & (1 << (col - 1)))) ++cnt;
                        /* right neighbour */
                        if (col < 2 && (row & (1 << (col + 1)))) ++cnt;
                        /* up neighbour */
                        if (up & (1 << col)) ++cnt;
                        /* down neighbour */
                        if (down & (1 << col)) ++cnt;
                        if (cnt > 2) ++v;
                    }
                    viol[up][row][down] = (unsigned char)v;
                }
            }
        }
        viol_inited = 1;
    }
}

/* DP arrays: dp[prev_prev][prev][v==0/1] */
static seq_t dp[8][8][2];
static seq_t nxt[8][8][2];

/* main function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_viol();

    if (n == 0) return 0;          /* safety for out‑of‑bound index */

    /* clear DP arrays */
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 2; ++k)
                dp[i][j][k] = 0, nxt[i][j][k] = 0;

    /* n==1: only one row, evaluate its violations directly */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < 8; ++row) {
            int v = viol[0][row][0];              /* no up, no down */
            if (v == 1) total += 1;
        }
        return total;
    }

    /* initial state with first two rows (row0, row1) */
    for (int row0 = 0; row0 < 8; ++row0) {
        for (int row1 = 0; row1 < 8; ++row1) {
            int v0 = viol[0][row0][row1];         /* top row has no up */
            if (v0 <= 1) dp[row0][row1][v0] += 1;
        }
    }

    /* process rows 2 .. n-1 */
    for (idx_t pos = 2; pos < n; ++pos) {
        /* clear nxt */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    nxt[i][j][k] = 0;
        for (int up = 0; up < 8; ++up) {
            for (int cur = 0; cur < 8; ++cur) {
                for (int v = 0; v <= 1; ++v) {
                    seq_t cnt = dp[up][cur][v];
                    if (cnt == 0) continue;
                    for (int down = 0; down < 8; ++down) {
                        int vrow = viol[up][cur][down];
                        int tot = v + vrow;
                        if (tot > 1) continue;
                        nxt[cur][down][tot] += cnt;
                    }
                }
            }
        }
        /* copy nxt to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 2; ++k)
                    dp[i][j][k] = nxt[i][j][k];
    }

    /* final row (last) has no down neighbour */
    seq_t result = 0;
    for (int up = 0; up < 8; ++up) {
        for (int row = 0; row < 8; ++row) {
            for (int v = 0; v <= 1; ++v) {
                seq_t cnt = dp[up][row][v];
                if (cnt == 0) continue;
                int vlast = viol[up][row][0];          /* no down */
                if (v + vlast == 1) result += cnt;
            }
        }
    }

    return result;
}
