/*
 * A298324  Number of nX4 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is computed by dynamic programming over *
 *            consecutive rows. Each 4‑bit row pattern is a binary assignment
 *            * to a 1×4 slice. A row is valid if for every cell, the number
 *            of * king‑move adjacent cells with the same value is in
 *            {0,2,3,4,6}. * The upper left cell is fixed to 0. The DP ke...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Allowed neighbor counts for a cell */
static const bool allowed[9] = {true, false, true, true, true, false, true, false, false};

/* Extract bit col (0..3) from a 4‑bit pattern */
static inline unsigned char bit_at(unsigned pattern, unsigned col)
{
    return (unsigned char)((pattern >> col) & 0x1U);
}

/* Check if a row 'curr' is valid given optional neighbours
 * 'prev' (row above) and 'next' (row below).  If prev_exists is
 * false then the above neighbours are omitted; similarly for
 * next_exists. */
static inline bool row_valid(unsigned prev, unsigned curr, unsigned next,
                             bool prev_exists, bool next_exists)
{
    for (unsigned c = 0; c < 4; ++c) {
        unsigned char cell = bit_at(curr, c);
        unsigned count = 0;

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) continue;
                int nc = (int)c + dc;
                if (nc < 0 || nc >= 4) continue;

                unsigned char neigh = 0;
                if (dr == -1) {
                    if (!prev_exists) continue;
                    neigh = bit_at(prev, (unsigned)nc);
                } else if (dr == 1) {
                    if (!next_exists) continue;
                    neigh = bit_at(next, (unsigned)nc);
                } else {
                    neigh = bit_at(curr, (unsigned)nc);
                }

                if (neigh == cell) ++count;
            }
        }
        if (!allowed[count]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1 only defined */

    /* Special case: single row */
    if (n == 1) {
        seq_t ans = 0;
        for (unsigned curr = 0; curr < 16; ++curr) {
            if ((curr & 1U) != 0) continue; /* upper‑left must be 0 */
            if (row_valid(0, curr, 0, false, false)) ans += 1;
        }
        return ans;
    }

    /* DP tables for pairs (prev_row, curr_row) */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    for (unsigned i = 0; i < 16; ++i)
        for (unsigned j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* Initialise with first two rows */
    for (unsigned row0 = 0; row0 < 16; ++row0) {
        if ((row0 & 1U) != 0) continue;          /* upper‑left zero */
        for (unsigned row1 = 0; row1 < 16; ++row1) {
            if (row_valid(0, row0, row1, false, true))
                dp[row0][row1] += 1;
        }
    }

    /* Transition over remaining rows */
    for (idx_t r = 2; r < n; ++r) {          /* r is index of new row */
        for (unsigned i = 0; i < 16; ++i)
            for (unsigned j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (unsigned prev = 0; prev < 16; ++prev) {
            for (unsigned curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                for (unsigned next = 0; next < 16; ++next) {
                    if (row_valid(prev, curr, next, true, true))
                        ndp[curr][next] += val;
                }
            }
        }

        /* copy ndp back to dp */
        for (unsigned i = 0; i < 16; ++i)
            for (unsigned j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row check (no row below) */
    seq_t ans = 0;
    for (unsigned prev = 0; prev < 16; ++prev) {
        for (unsigned curr = 0; curr < 16; ++curr) {
            seq_t val = dp[prev][curr];
            if (!val) continue;
            if (row_valid(prev, curr, 0, true, false))
                ans += val;
        }
    }

    return ans;
}
