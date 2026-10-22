/*
 * A302306  Number of nX5 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This implementation uses dynamic programming over row states.
 *            Each row is a 5‑bit binary number. A row state is represented by
 *            an integer 0..31. The DP counts how many ways to build an n×5
 *            binary array satisfying the neighbour‑count rule with the
 *            upper‑left cell fixed to 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Pre‑computed validity tables.  row_ok(…) checks whether a given
   row (currow) satisfies the neighbour rule when the rows above
   and below are (prevrow) and (nextrow), respectively.
   The parameters hasPrev/hasNext indicate whether the corresponding
   neighbour rows exist. */
static bool top_ok[32][32];
static bool mid_ok[32][32][32];
static bool end_ok[32][32];
static bool single_ok[32];
static bool tables_ready = false;

/* Check a single cell of a row assuming currow (0..31) and neighbour
   rows prevrow, nextrow.  All other bits are ignored. */
static bool row_ok(uint8_t prevrow, uint8_t currow, uint8_t nextrow,
                   bool hasPrev, bool hasNext)
{
    for (int j = 0; j < 5; ++j) {
        int bit = (currow >> j) & 1;
        int cnt = 0;

        /* horizontal neighbours */
        if (j > 0)  cnt += (((currow >> (j - 1)) & 1) == bit);
        if (j < 4)  cnt += (((currow >> (j + 1)) & 1) == bit);

        /* diagonal neighbours */
        if (hasPrev) {
            if (j > 0)  cnt += (((prevrow >> (j - 1)) & 1) == bit);
            if (j < 4)  cnt += (((prevrow >> (j + 1)) & 1) == bit);
        }
        if (hasNext) {
            if (j > 0)  cnt += (((nextrow >> (j - 1)) & 1) == bit);
            if (j < 4)  cnt += (((nextrow >> (j + 1)) & 1) == bit);
        }

        /* allowed neighbour counts are 0,1,3,4 */
        if (!(cnt == 0 || cnt == 1 || cnt == 3 || cnt == 4))
            return false;
    }
    return true;
}

static void build_tables(void)
{
    if (tables_ready) return;
    for (int i = 0; i < 32; ++i) {
        single_ok[i] = row_ok(0, (uint8_t)i, 0, false, false);
        for (int j = 0; j < 32; ++j) {
            /* first row: no upper neighbour, top neighbour exists */
            top_ok[i][j] = row_ok(0, (uint8_t)i, (uint8_t)j, false, true);
            /* last row: lower neighbour missing */
            end_ok[i][j] = row_ok((uint8_t)i, (uint8_t)j, 0, true, false);
            for (int k = 0; k < 32; ++k) {
                /* interior rows: both neighbours exist */
                mid_ok[i][j][k] = row_ok((uint8_t)i, (uint8_t)j,
                                         (uint8_t)k, true, true);
            }
        }
    }
    tables_ready = true;
}

/* The main sequence generator. */
seq_t a(idx_t n)
{
    build_tables();

    if (n == 0) return 0;            /* undefined, safe fallback */

    if (n == 1) {
        seq_t total = 0;
        for (int r = 0; r < 32; ++r) {
            if ((r & 1) == 0 && single_ok[r])
                total += 1;
        }
        return total;
    }

    /* DP tables: dp[prev][curr] holds counts of arrays up to row 'curr'.
       We keep two layers to avoid overwriting. */
    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* initialise dp with all row1–row2 pairs that satisfy the first
       neighbour rule (row1 checked against row2). */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;
    for (int r1 = 0; r1 < 32; ++r1) {
        if ((r1 & 1) != 0) continue;          /* upper‑left cell must be 0 */
        for (int r2 = 0; r2 < 32; ++r2) {
            if (top_ok[r1][r2]) dp[r1][r2] = 1;
        }
    }

    /* iterate from row 3 up to row n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next layer */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = 0;

        /* transition: dp[prev][curr] -> ndp[curr][next] if mid_ok holds */
        for (int rprev = 0; rprev < 32; ++rprev) {
            for (int rcurr = 0; rcurr < 32; ++rcurr) {
                seq_t ways = dp[rprev][rcurr];
                if (ways == 0) continue;
                for (int rnext = 0; rnext < 32; ++rnext) {
                    if (mid_ok[rprev][rcurr][rnext])
                        ndp[rcurr][rnext] += ways;
                }
            }
        }
        /* move next layer into current */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final summation: last row must satisfy end_ok with its predecessor */
    seq_t total = 0;
    for (int rprev = 0; rprev < 32; ++rprev) {
        for (int rlast = 0; rlast < 32; ++rlast) {
            if (end_ok[rprev][rlast])
                total += dp[rprev][rlast];
        }
    }
    return total;
}
