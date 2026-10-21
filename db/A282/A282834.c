/*
 * A282834  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with exactly two cells that
 *            have more than one king‑move neighbor equal to 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following uses a DP over rows.  For every pair of consecutive
   rows (top, middle) we keep the number of ways to reach that state
   and the number of bad cells seen so far (0, 1 or 2).  When a new
   row is added we finalize the middle row using the three rows:
   top, middle and bottom.  At the end we also finalize the last
   row with a virtual bottom row of all zeros.  */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑compute the number of bad cells in the middle row of a triple of
   rows.  */
static uint8_t bad[16][16][16];
static bool bad_inited = false;

static void init_bad(void)
{
    if (bad_inited) return;
    for (int top = 0; top < 16; ++top)
        for (int mid = 0; mid < 16; ++mid)
            for (int bot = 0; bot < 16; ++bot) {
                uint8_t cnt = 0;
                for (int col = 0; col < 4; ++col) {
                    if (!((mid >> col) & 1)) continue;   /* cell not 1 */
                    int neigh = 0;
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;
                            int c = col + dc;
                            if (c < 0 || c > 3) continue;
                            if (dr == -1) neigh += (top >> c) & 1;
                            else if (dr == 0) neigh += (mid >> c) & 1;
                            else neigh += (bot >> c) & 1;
                        }
                    }
                    if (neigh > 1) ++cnt;
                }
                bad[top][mid][bot] = cnt;
            }
    bad_inited = true;
}

/* DP over rows.  The DP table has dimensions [2][16][16][3].  */
static seq_t dp[2][16][16][3];

/* Main function as required. */
seq_t a(idx_t n)
{
    init_bad();

    /* initialise DP */
    for (int t = 0; t < 16; ++t)
        for (int m = 0; m < 16; ++m)
            for (int c = 0; c < 3; ++c)
                dp[0][t][m][c] = 0;
    dp[0][0][0][0] = 1;          /* start with two dummy zero rows */

    int cur = 0, nxt = 1;

    for (idx_t row = 0; row < n; ++row) {
        /* zero next table */
        for (int t = 0; t < 16; ++t)
            for (int m = 0; m < 16; ++m)
                for (int c = 0; c < 3; ++c)
                    dp[nxt][t][m][c] = 0;

        for (int top = 0; top < 16; ++top)
            for (int mid = 0; mid < 16; ++mid)
                for (int c = 0; c < 3; ++c) {
                    seq_t ways = dp[cur][top][mid][c];
                    if (ways == 0) continue;
                    for (int curRow = 0; curRow < 16; ++curRow) {
                        uint8_t bc = bad[top][mid][curRow];
                        uint8_t nc = c + bc;
                        if (nc <= 2)
                            dp[nxt][mid][curRow][nc] = dp[nxt][mid][curRow][nc] + ways;
                    }
                }
        /* swap tables */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* finalisation with a dummy bottom row of zeros */
    seq_t ans = 0;
    for (int top = 0; top < 16; ++top)
        for (int mid = 0; mid < 16; ++mid)
            for (int c = 0; c < 3; ++c) {
                seq_t ways = dp[cur][top][mid][c];
                if (ways == 0) continue;
                uint8_t bc = bad[top][mid][0];
                int finalc = c + bc;
                if (finalc == 2)
                    ans = ans + ways;
            }

    return ans;
}
