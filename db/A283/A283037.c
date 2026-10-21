/*
 * A283037  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          one element.
 * https://oeis.org/A283037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283037: number of n × 3 binary arrays with at most one 1 that has
 *  more than one 1‑neighbour (horizontal or vertical).  This implementation
 *  uses dynamic programming over the rows.  The state keeps the last two
 *  rows and a flag indicating whether the single allowed violation has
 *  already occurred.  Complexity: O(n) with a small constant.  */
#include <stdbool.h>

static signed char vio[9][9][9];
static bool vio_inited = false;

seq_t a(idx_t n)
{
    /* initialise the table of violations for any triple of rows
     * (index 8 is a dummy row that behaves like all zeros).  */
    if (!vio_inited) {
        vio_inited = true;
        for (int up = 0; up < 9; ++up) {
            int a_bits = (up == 8) ? 0 : up;          /* above row      */
            for (int mid = 0; mid < 9; ++mid) {
                int m_bits = (mid == 8) ? 0 : mid;    /* middle row     */
                for (int down = 0; down < 9; ++down) {
                    int b_bits = (down == 8) ? 0 : down;
                    int cnt = 0;
                    for (int c = 0; c < 3; ++c) {
                        if (m_bits & (1 << c)) {
                            int nbs = 0;
                            /* left neighbour */
                            if (c > 0 && (m_bits & (1 << (c - 1)))) nbs++;
                            /* right neighbour */
                            if (c < 2 && (m_bits & (1 << (c + 1)))) nbs++;
                            /* vertical neighbours */
                            if (a_bits & (1 << c)) nbs++;
                            if (b_bits & (1 << c)) nbs++;
                            if (nbs > 1) cnt++;          /* violating cell */
                        }
                    }
                    vio[up][mid][down] = (signed char)cnt;
                }
            }
        }
    }

    /* DP arrays: state(prevprev, prev, usedFlag) -> count */
    static seq_t dp[9][9][2];
    static seq_t dp_next[9][9][2];

    /* clear dp for this call */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            for (int k = 0; k < 2; ++k)
                dp[i][j][k] = 0;

    /* start with two dummy rows (index 8). */
    dp[8][8][0] = 1;

    /* process each row */
    for (idx_t row = 0; row < n; ++row) {
        /* clear dp_next */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                for (int k = 0; k < 2; ++k)
                    dp_next[i][j][k] = 0;

        for (int up = 0; up < 9; ++up) {
            for (int mid = 0; mid < 9; ++mid) {
                for (int used = 0; used < 2; ++used) {
                    seq_t cur = dp[up][mid][used];
                    if (!cur) continue;
                    /* try each possible new row (0..7) */
                    for (int new_row = 0; new_row < 8; ++new_row) {
                        int vioCnt = vio[up][mid][new_row];
                        int new_used = used + vioCnt;
                        if (new_used <= 1) {
                            dp_next[mid][new_row][new_used] += cur;
                        }
                    }
                }
            }
        }

        /* copy dp_next -> dp for next iteration */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                for (int k = 0; k < 2; ++k)
                    dp[i][j][k] = dp_next[i][j][k];
    }

    /* finally, account for the last row with a dummy bottom neighbour */
    seq_t ans = 0;
    for (int up = 0; up < 9; ++up) {
        for (int mid = 0; mid < 9; ++mid) {
            for (int used = 0; used < 2; ++used) {
                seq_t cur = dp[up][mid][used];
                if (!cur) continue;
                int vioCnt = vio[up][mid][8];   /* below dummy row */
                int new_used = used + vioCnt;
                if (new_used == 1)
                    ans += cur;
            }
        }
    }
    return ans;
}
