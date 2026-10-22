/*
 * A303687  Number of nX5 0..1 arrays with every element unequal to 2, 3 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays (with upper‑left cell 0) in
 *            which each element has 2, 3 or 4 king‑move adjacent cells that
 *            are unequal to it. The value is computed by a transfer‑matrix DP
 *            over rows
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool valid_top[32][32];
static bool valid_middle[32][32][32];
static bool valid_bottom[32][32];

/* Test whether the row 'curr' is legal when it has the optional
   neighbours *above* and *below* (present flags).  */
static bool row_valid(bool above_present, uint8_t above,
                      uint8_t curr, bool below_present, uint8_t below)
{
    for (int j = 0; j < 5; ++j) {
        int val = (curr >> j) & 1;
        int unequal = 0;

        /* ---------- neighbours in the same row ---------- */
        if (j > 0)  { int v = (curr >> (j-1)) & 1; if (v != val) ++unequal; }
        if (j < 4)  { int v = (curr >> (j+1)) & 1; if (v != val) ++unequal; }

        /* ---------- neighbours in the row above ---------- */
        if (above_present) {
            int v = (above >> j) & 1;           if (v != val) ++unequal;
            if (j > 0)  { v = (above >> (j-1)) & 1; if (v != val) ++unequal; }
            if (j < 4)  { v = (above >> (j+1)) & 1; if (v != val) ++unequal; }
        }

        /* ---------- neighbours in the row below ---------- */
        if (below_present) {
            int v = (below >> j) & 1;           if (v != val) ++unequal;
            if (j > 0)  { v = (below >> (j-1)) & 1; if (v != val) ++unequal; }
            if (j < 4)  { v = (below >> (j+1)) & 1; if (v != val) ++unequal; }
        }

        if (unequal < 2 || unequal > 4) return false;
    }
    return true;
}

/* Pre‑compute all validity tables.  The tables are used only once. */
static void init_tables(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    for (int curr = 0; curr < 32; ++curr) {
        for (int below = 0; below < 32; ++below) {
            valid_top[curr][below] = row_valid(false, 0, curr, true, below);
        }
    }

    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            for (int below = 0; below < 32; ++below) {
                valid_middle[prev][curr][below] = row_valid(true, prev, curr, true, below);
            }
            valid_bottom[prev][curr] = row_valid(true, prev, curr, false, 0);
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    if (n == 0) return 0;
    if (n == 1) {
        seq_t ans = 0;
        for (int row = 0; row < 32; ++row) {
            if ((row & 1) == 0 && row_valid(false, 0, row, false, 0))
                ans += 1;
        }
        return ans;
    }

    /* DP tables */
    seq_t dp[32][32];
    seq_t newdp[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = newdp[i][j] = 0;

    /* Initialisation: first two rows */
    for (int r0 = 0; r0 < 32; ++r0) {
        if ((r0 & 1) != 0) continue;          /* upper‑left cell must be 0 */
        for (int r1 = 0; r1 < 32; ++r1) {
            if (valid_top[r0][r1]) dp[r0][r1] = 1;
        }
    }

    /* Process rows 2 ... n-1 */
    for (uint64_t row = 2; row < n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                for (int next = 0; next < 32; ++next) {
                    if (valid_middle[prev][curr][next])
                        newdp[curr][next] += ways;
                }
            }
        }
        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final row validation (bottom) */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (valid_bottom[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
