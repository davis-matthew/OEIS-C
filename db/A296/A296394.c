/*
 * A296394  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A296394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*********************************************************************
 * Sequence A296394
 * 
 * Compute the number of n×6 binary arrays in which every 1 has
 * exactly 1 or 3 neighboring 1s among the 6 directions:
 *   left, right, up, down, up‑right, down‑left.
 *
 * The computation uses a transfer‑matrix DP over rows of width 6.
 * Each state is a pair (previous_row, current_row).  While adding a
 * new row we check the constraints for the middle row, thereby
 * ensuring that all rows except the last are already valid.
 * A virtual zero row is appended at the boundaries to handle the
 * edge cells.
 *********************************************************************/

#include <stdbool.h>
#include <stdint.h>

/*------------------------------------------------------------------------
 * Helper: check whether a given row satisfies the neighbour rule
 *         when its upper row is `prev` and lower row is `next`.
 * `prev`, `cur`, `next` are 6‑bit numbers encoding a row.
 *-----------------------------------------------------------------------*/
static bool row_valid(int prev, int cur, int next)
{
    for (int j = 0; j < 6; ++j) {
        if (!(cur & (1 << j)))                /* cell is 0, no constraint   */
            continue;

        int cnt = 0;
        /* horizontal neighbours */
        if (j > 0   && (cur & (1 << (j - 1)))) cnt++;   /* left   */
        if (j < 5   && (cur & (1 << (j + 1)))) cnt++;   /* right  */
        /* vertical neighbours */
        if (prev & (1 << j))   cnt++;                   /* up     */
        if (next & (1 << j))   cnt++;                   /* down   */
        /* antidiagonal neighbours */
        if (j < 5   && (prev & (1 << (j + 1)))) cnt++; /* up‑right */
        if (j > 0   && (next & (1 << (j - 1)))) cnt++; /* down‑left */

        if (cnt != 1 && cnt != 3)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------
 * Main sequence function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Precompute which next rows keep `cur` valid given `prev`. */
    static bool initialized = false;
    static uint64_t validMask[64][64];
    static seq_t dp[64][64];
    static seq_t dp2[64][64];

    if (!initialized) {
        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                uint64_t mask = 0;
                for (int next = 0; next < 64; ++next)
                    if (row_valid(prev, cur, next))
                        mask |= (uint64_t)1 << next;
                validMask[prev][cur] = mask;
            }
        }
        initialized = true;
    }

    /* Initialise DP after the zeroth row (all zeros). */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[i][j] = 0;
    for (int cur = 0; cur < 64; ++cur)
        dp[0][cur] = 1;

    /* Process rows 2 … n.  At each step we add a new row and
       check the previous row with the new information. */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp2[i][j] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int cur = 0; cur < 64; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways)
                    continue;
                uint64_t mask = validMask[prev][cur];
                for (int next = 0; next < 64; ++next)
                    if (mask & ((uint64_t)1 << next))
                        dp2[cur][next] += ways;
            }
        }

        /* Move dp2 into dp for the next iteration. */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = dp2[i][j];
    }

    /* Final row check: append a zero row below. */
    seq_t total = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int cur = 0; cur < 64; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways)
                continue;
            if (row_valid(prev, cur, 0))
                total += ways;
        }
    }
    return total;
}
