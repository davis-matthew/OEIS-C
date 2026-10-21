/*
 * A295527  Number of nX4 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 2 or 4 1s.
 * https://oeis.org/A295527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which each 1 has 1, 2 or 4
 *            horizontal/vertical neighbors. The sequence is computed by
 *            dynamic programming over the rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Auxiliary: precompute horizontal neighbor counts for each 4‑bit pattern. */
static int hdeg[16][4];
static bool hdeg_inited = false;
static void init_hdeg(void)
{
    if (hdeg_inited) return;
    for (int p = 0; p < 16; ++p) {
        for (int j = 0; j < 4; ++j) {
            int cnt = 0;
            if (j > 0 && ((p >> (j - 1)) & 1)) ++cnt;
            if (j < 3 && ((p >> (j + 1)) & 1)) ++cnt;
            hdeg[p][j] = cnt;
        }
    }
    hdeg_inited = true;
}

/* Check whether the row 'cur' satisfies the adjacency rule given its
   up and down neighbours.  For each 1 in cur compute
   partial = horizontal part + up bit, and verify that
   final = partial + down bit is in {1,2,4}. */
static bool check_transition(int up, int cur, int down)
{
    for (int j = 0; j < 4; ++j) {
        if ((cur >> j) & 1) {
            int partial = hdeg[cur][j] + ((up >> j) & 1);
            int final   = partial + ((down >> j) & 1);
            if (!(final == 1 || final == 2 || final == 4))
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    init_hdeg();

    if (n == 0) return 0;          /* offset has no meaning for n<1 */

    /* ===== n == 1 : handle directly ===== */
    if (n == 1) {
        seq_t res = 0;
        for (int p = 0; p < 16; ++p) {
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                if ((p >> j) & 1) {
                    int deg = hdeg[p][j];      /* no vertical neighbours */
                    if (!(deg == 1 || deg == 2)) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) res = res + 1;
        }
        return res;
    }

    /* ===== General case n >= 2 ===== */
    static seq_t dp[16][16];
    static seq_t dp_next[16][16];

    /* Initialise dp with pairs (row0, row1) that satisfy row0 constraints */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (int p0 = 0; p0 < 16; ++p0) {
        for (int p1 = 0; p1 < 16; ++p1) {
            if (check_transition(0, p0, p1))
                dp[p0][p1] = dp[p0][p1] + 1;
        }
    }

    /* Process rows 2 .. n-1 */
    for (idx_t i = 2; i < n; ++i) {
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                dp_next[a][b] = 0;

        for (int up = 0; up < 16; ++up) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t ways = dp[up][cur];
                if (ways == 0) continue;
                for (int down = 0; down < 16; ++down) {
                    if (check_transition(up, cur, down))
                        dp_next[cur][down] = dp_next[cur][down] + ways;
                }
            }
        }

        /* swap dp and dp_next */
        for (int a = 0; a < 16; ++a)
            for (int b = 0; b < 16; ++b)
                dp[a][b] = dp_next[a][b];
    }

    /* Final row has no below neighbour */
    seq_t result = 0;
    for (int up = 0; up < 16; ++up) {
        for (int cur = 0; cur < 16; ++cur) {
            seq_t ways = dp[up][cur];
            if (ways == 0) continue;
            if (check_transition(up, cur, 0))
                result = result + ways;
        }
    }

    return result;
}
