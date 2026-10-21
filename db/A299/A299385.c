/*
 * A299385  Number of nX5 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence this code implements:
   A299385 – Number of n×5 0/1 arrays with every element equal to 0,1,2,4,6 or 8
   king‑move adjacent elements, with the upper‑left element zero.
   The implementation uses dynamic programming over rows, validating each
   row when both its upper and lower neighbours are known. */

#include <stdbool.h>
#include <stdint.h>

/* Allowed neighbour‑equal counts for a cell. */
static const bool allowed[9] = {
    /* 0 1 2 3 4 5 6 7 8 */
    true, true, true, false, true, false, true, false, true
};

/* Returns true iff every cell of the middle row 'mid' has a number of
   king‑move neighbours equal to itself that lies in the allowed set.
   The flags 'has_up' and 'has_down' indicate whether the corresponding
   neighbour rows actually exist (outside the grid otherwise). */
static bool row_valid(uint32_t mid, uint32_t up, uint32_t down,
                      bool has_up, bool has_down)
{
    for (int c = 0; c < 5; ++c) {
        uint32_t mid_bit = (mid >> c) & 1U;
        int cnt = 0;

        /* Upper neighbours */
        if (has_up) {
            if (((up >> c) & 1U) == mid_bit)      cnt++; /* up‑center   */
            if (c > 0   && ((up >> (c - 1)) & 1U) == mid_bit) cnt++; /* up‑left  */
            if (c < 4   && ((up >> (c + 1)) & 1U) == mid_bit) cnt++; /* up‑right */
        }

        /* Middle row neighbours (left/right) – always present */
        if (c > 0   && ((mid >> (c - 1)) & 1U) == mid_bit) cnt++; /* left  */
        if (c < 4   && ((mid >> (c + 1)) & 1U) == mid_bit) cnt++; /* right */

        /* Lower neighbours */
        if (has_down) {
            if (((down >> c) & 1U) == mid_bit)      cnt++; /* down‑center */
            if (c > 0   && ((down >> (c - 1)) & 1U) == mid_bit) cnt++; /* down‑left */
            if (c < 4   && ((down >> (c + 1)) & 1U) == mid_bit) cnt++; /* down‑right */
        }

        if (!allowed[cnt]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside OEIS index range */

    /* Special case n = 1: only one row exists, no neighbours above or below. */
    if (n == 1) {
        seq_t total = 0;
        for (uint32_t row = 0; row < 32; ++row) {
            if ((row & 1U) == 0) {                    /* upper‑left zero */
                if (row_valid(row, 0, 0, false, false))
                    total += (seq_t)1;
            }
        }
        return total;
    }

    /* dp[prev][cur] = number of ways to build rows up to 'cur',
       with previous row = 'prev' and current row = 'cur'. */
    seq_t dp[32][32] = {0};
    seq_t ndp[32][32] = {0};

    /* Initialise for the first two rows (row 1 and row 2). */
    for (uint32_t r1 = 0; r1 < 32; ++r1) {
        if ((r1 & 1U) != 0) continue;   /* upper‑left element must be 0 */
        for (uint32_t r2 = 0; r2 < 32; ++r2) {
            if (row_valid(r1, 0, r2, false, true))
                dp[r1][r2] = (seq_t)1;
        }
    }

    /* Add rows 3 through n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear ndp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                ndp[i][j] = (seq_t)0;

        /* transition */
        for (uint32_t up = 0; up < 32; ++up) {
            for (uint32_t mid = 0; mid < 32; ++mid) {
                seq_t cnt = dp[up][mid];
                if (!cnt) continue;
                for (uint32_t down = 0; down < 32; ++down) {
                    if (row_valid(mid, up, down, true, true))
                        ndp[mid][down] += cnt;
                }
            }
        }

        /* copy ndp back to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row (row n) has no row below. */
    seq_t total = 0;
    for (uint32_t up = 0; up < 32; ++up) {
        for (uint32_t mid = 0; mid < 32; ++mid) {
            seq_t cnt = dp[up][mid];
            if (!cnt) continue;
            if (row_valid(mid, up, 0, true, false))
                total += cnt;
        }
    }

    return total;
}
