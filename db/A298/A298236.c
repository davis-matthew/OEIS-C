/*
 * A298236  Number of nX4 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298236: Number of n×4 binary arrays with the king‑move
   neighbour‑constraint (0,2,3,4,5 equal neighbours per cell) and
   with the upper left element fixed to 0.
   Computed by a transfer‑matrix dynamic programming over two consecutive
   rows.  Initial values (for n = 1 … 20) are:
   1, 42, 124, 1156, 8117, 66626, 563826, 4788445, 41239301, 355478656,
   3071723340, 26560212799, 229752597446, 1987833161737, 17200321179989,
   148838661721030, 1287965419617046, 11145454952537116, 96448124237702960,
   834624171283503694.  */

#include <stdint.h>

#define NONE 16U  /* sentinel for “no previous row” */

/* Check whether a single cell satisfies the neighbour‑count rule. */
static inline int valid_cell(idx_t prev_row, idx_t curr_row, idx_t next_row,
                             int col, int val)
{
    int neigh = 0;

    /* Same‑row neighbours (left/right). */
    if (col > 0 && ((curr_row >> (col - 1)) & 1) == val) neigh++;
    if (col < 3 && ((curr_row >> (col + 1)) & 1) == val) neigh++;

    /* Upper neighbours (prev row). */
    if (prev_row != NONE) {
        if (((prev_row >> col) & 1) == val) neigh++;                /* up */
        if (col > 0 && ((prev_row >> (col - 1)) & 1) == val) neigh++; /* up‑left */
        if (col < 3 && ((prev_row >> (col + 1)) & 1) == val) neigh++; /* up‑right */
    }

    /* Lower neighbours (next row). */
    if (next_row != NONE) {
        if (((next_row >> col) & 1) == val) neigh++;                /* down */
        if (col > 0 && ((next_row >> (col - 1)) & 1) == val) neigh++; /* down‑left */
        if (col < 3 && ((next_row >> (col + 1)) & 1) == val) neigh++; /* down‑right */
    }

    /* Allowed neighbour counts: 0, 2, 3, 4, 5. */
    return (neigh == 0 || neigh == 2 || neigh == 3 || neigh == 4 || neigh == 5);
}

/* Verify that an entire row is valid given its upper and lower neighbours. */
static inline int row_valid(idx_t prev, idx_t curr, idx_t next)
{
    for (int col = 0; col < 4; ++col) {
        int val = (int)((curr >> col) & 1);
        if (!valid_cell(prev, curr, next, col, val))
            return 0;
    }
    return 1;
}

/* Pre‑compute all admissible transitions: trans[prev][curr] is a
   16‑bit mask of next rows for which row_valid(prev,curr,next) holds. */
static void precompute_mask(uint16_t trans[17][16])
{
    for (int prev = 0; prev <= 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            uint16_t mask = 0;
            for (int next = 0; next < 16; ++next) {
                if (row_valid(prev, curr, next))
                    mask |= 1U << next;
            }
            trans[prev][curr] = mask;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 */

    /* DP state: dp[prev][curr] holds the count of partial grids
       ending with the previous row “prev” and the current row “curr”. */
    seq_t dp[17][16] = {0}, ndp[17][16] = {0};
    uint16_t trans[17][16];
    precompute_mask(trans);

    /* Initial state – only the first row is known. */
    for (int curr = 0; curr < 16; ++curr) {
        if ((curr & 1) == 0)          /* upper‑left bit must be 0 */
            dp[NONE][curr] = 1;
    }

    /* Process rows 2 … n. */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next DP layer */
        for (int p = 0; p <= 16; ++p)
            for (int c = 0; c < 16; ++c)
                ndp[p][c] = 0;

        for (int prev = 0; prev <= 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == 0) continue;
                uint16_t mask = trans[prev][curr];
                for (int next = 0; next < 16; ++next) {
                    if (mask & (1U << next))
                        ndp[curr][next] += val;
                }
            }
        }

        /* copy ndp to dp for the next iteration */
        for (int p = 0; p <= 16; ++p)
            for (int c = 0; c < 16; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* Final validation – the last row has no lower neighbours. */
    seq_t result = 0;
    for (int prev = 0; prev <= 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            seq_t val = dp[prev][curr];
            if (val == 0) continue;
            if (row_valid(prev, curr, NONE))
                result += val;
        }
    }

    return result;
}
