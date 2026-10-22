/*
 * A300642  Number of nX4 0..1 arrays with every element equal to 2, 3, 4 or 5
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 4 binary arrays where each cell has *
 *            between 2 and 5 adjacent cells (horizontally, vertically or *
 *            anti-diagonally) of the same value, with the upper‑left cell
 *            fixed at 0. * The function implements a dynamic‑programming
 *            algorithm over rows
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Return true if one row satisfies the neighbor constraint (row numbers are
 * 4‑bit values).  above_exists / below_exists indicate whether a row
 * exists above / below the current row.  Diagonals counted are only the
 * anti‑diagonal ones: (i-1,j+1) and (i+1,j-1). */
static inline bool row_valid(unsigned int above,
                             unsigned int current,
                             unsigned int below,
                             bool above_exists,
                             bool below_exists)
{
    for (int col = 0; col < 4; ++col) {
        unsigned int val = (current >> col) & 1U;
        unsigned int cnt = 0;

        /* horizontal neighbours */
        if (col > 0 && ((current >> (col - 1)) & 1U) == val) ++cnt;
        if (col < 3 && ((current >> (col + 1)) & 1U) == val) ++cnt;

        /* vertical neighbours */
        if (above_exists && ((above >> col) & 1U) == val) ++cnt;
        if (below_exists && ((below >> col) & 1U) == val) ++cnt;

        /* anti‑diagonal neighbours */
        /* from above row (i-1, j+1) */
        if (above_exists && col < 3 && ((above >> (col + 1)) & 1U) == val)
            ++cnt;
        /* from below row (i+1, j-1) */
        if (below_exists && col > 0 && ((below >> (col - 1)) & 1U) == val)
            ++cnt;

        if (cnt < 2 || cnt > 5) return false;
    }
    return true;
}

/* Pre‑compute adjacency tables used in the DP. */
static void init_tables(bool ok_first[16][16],
                        bool ok_middle[16][16][16],
                        bool ok_last[16][16])
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    for (int a = 0; a < 16; ++a) {
        for (int b = 0; b < 16; ++b) {
            ok_first[a][b] = row_valid(0U, (unsigned int)a, (unsigned int)b,
                                       false, true);
            ok_last[a][b]  = row_valid((unsigned int)a, (unsigned int)b,
                                       0U, true, false);
            for (int c = 0; c < 16; ++c) {
                ok_middle[a][b][c] = row_valid((unsigned int)a,
                                               (unsigned int)b,
                                               (unsigned int)c,
                                               true, true);
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Undefined for n = 0, but avoid wrap‑around for loops. */
    if (n == 0) return 0;

    static bool ok_first[16][16];
    static bool ok_middle[16][16][16];
    static bool ok_last[16][16];
    init_tables(ok_first, ok_middle, ok_last);

    /* Handle the trivial single‑row case. */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < 16; ++row) {
            /* upper‑left cell (bit 0) must be 0 */
            if ((row & 1) != 0) continue;
            if (row_valid(0U, (unsigned int)row, 0U, false, false))
                total += (seq_t)1;
        }
        return total;
    }

    /* DP arrays: dp[prev][curr] gives number of ways up to the current row. */
    static seq_t dp[16][16];
    static seq_t nextdp[16][16];

    /* Initialize dp for the first two rows. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    for (int r1 = 0; r1 < 16; ++r1) {
        if ((r1 & 1) != 0) continue;            /* upper‑left zero */
        for (int r2 = 0; r2 < 16; ++r2) {
            if (ok_first[r1][r2])
                dp[r1][r2] = (seq_t)1;
        }
    }

    /* Build rows 3 through n. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero nextdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nextdp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int next = 0; next < 16; ++next) {
                    if (ok_middle[prev][curr][next]) {
                        nextdp[curr][next] += ways;
                    }
                }
            }
        }
        /* Copy nextdp to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* Count valid final rows. */
    seq_t result = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t ways = dp[prev][curr];
            if (ways && ok_last[prev][curr])
                result += ways;
        }
    }

    return result;
}
