/*
 * A268746  Number of nX4 binary arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268746 – number of n×4 binary arrays with at most one
   adjacent pair (horizontally or vertically) that sums to 2.  The
   algorithm performs a DP over rows, keeping track of the last row's
   mask and the cumulative count of neighbouring pairs (0 or 1).  The
   implementation avoids any struct-copying that might double‑free an
   internal heap of big integer types. */

#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Precompute horizontal pairs in each 4‑bit mask and vertical
       pairs between two masks.  The tables are built only once. */
    static int horiz[16];
    static int vert[16][16];
    static bool inited = false;

    if (!inited) {
        for (int m = 0; m < 16; ++m) {
            int h = 0;
            for (int j = 0; j < 3; ++j)
                if ((m & (1 << j)) && (m & (1 << (j + 1)))) ++h;
            horiz[m] = h;
            for (int nmask = 0; nmask < 16; ++nmask) {
                int v = 0;
                for (int j = 0; j < 4; ++j)
                    if ((m & (1 << j)) && (nmask & (1 << j))) ++v;
                vert[m][nmask] = v;
            }
        }
        inited = true;
    }

    /* DP tables: dp_prev[mask][c] and dp_curr[mask][c] where
       c ∈ {0,1} counts the number of adjacent pairs found so far. */
    seq_t dp_prev[16][2];
    seq_t dp_curr[16][2];

    /* Initialise the tables to zero (use assignment, not memset). */
    for (int mask = 0; mask < 16; ++mask)
        for (int c = 0; c < 2; ++c) {
            dp_prev[mask][c] = (seq_t)0;
            dp_curr[mask][c] = (seq_t)0;
        }

    /* Base case – first row.  Only horizontal pairs exist. */
    for (int mask = 0; mask < 16; ++mask) {
        int h = horiz[mask];
        if (h <= 1)
            dp_prev[mask][h] = (seq_t)1;
    }

    /* Process remaining rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Reset current table to zero. */
        for (int mask = 0; mask < 16; ++mask)
            for (int c = 0; c < 2; ++c)
                dp_curr[mask][c] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                int h = horiz[curr];
                int v = vert[prev][curr];
                for (int c = 0; c < 2; ++c) {
                    seq_t val = dp_prev[prev][c];
                    if (val == (seq_t)0) continue;
                    int nc = c + h + v;
                    if (nc <= 1)
                        dp_curr[curr][nc] = dp_curr[curr][nc] + val;
                }
            }
        }

        /* Copy dp_curr into dp_prev for the next iteration. */
        for (int mask = 0; mask < 16; ++mask)
            for (int c = 0; c < 2; ++c)
                dp_prev[mask][c] = dp_curr[mask][c];
    }

    /* Sum all valid ending states. */
    seq_t result = (seq_t)0;
    for (int mask = 0; mask < 16; ++mask)
        for (int c = 0; c < 2; ++c)
            result = result + dp_prev[mask][c];

    return result;
}
