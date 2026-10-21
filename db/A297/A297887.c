/*
 * A297887  Number of nX6 0..1 arrays with every element equal to 0, 1, 2 or 4
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 44, u64 n <= 94, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297887: a(n) = number of n×6 binary arrays (entries 0 or 1) whose
 * upper‑left element is 0 and for every element the number of its
 * king‑move adjacent elements with the same value is 0, 1, 2, or 4.
 * Computed by dynamic programming over rows. */

#include <stdint.h>
#include <stdbool.h>

/* number of columns */
#define COLS   6
/* number of possible row masks (binary patterns of length COLS) */
#define ROWS   (1U << COLS)
/* sentinel value for “no row above / below” */
#define NO     ROWS          /* 64 */

static inline bool row_is_valid(uint64_t up_mask, uint64_t row_mask,
                                uint64_t down_mask, bool has_up,
                                bool has_down)
{
    for (int col = 0; col < COLS; ++col) {
        uint64_t cell = (row_mask >> col) & 1ULL;
        int cnt = 0;

        /* left and right in the same row */
        if (col > 0)
            if (((row_mask >> (col - 1)) & 1ULL) == cell) ++cnt;
        if (col < COLS - 1)
            if (((row_mask >> (col + 1)) & 1ULL) == cell) ++cnt;

        /* neighbours in the previous row */
        if (has_up) {
            if (col > 0)
                if (((up_mask >> (col - 1)) & 1ULL) == cell) ++cnt;
            if (((up_mask >> col) & 1ULL) == cell) ++cnt;
            if (col < COLS - 1)
                if (((up_mask >> (col + 1)) & 1ULL) == cell) ++cnt;
        }

        /* neighbours in the next row */
        if (has_down) {
            if (col > 0)
                if (((down_mask >> (col - 1)) & 1ULL) == cell) ++cnt;
            if (((down_mask >> col) & 1ULL) == cell) ++cnt;
            if (col < COLS - 1)
                if (((down_mask >> (col + 1)) & 1ULL) == cell) ++cnt;
        }

        /* allowed counts: 0, 1, 2 or 4 */
        if (!((cnt == 0) || (cnt == 1) || (cnt == 2) || (cnt == 4)))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Precompute transition masks: trans[up][row] is a 64‑bit mask of
     * admissible down rows when the current row is 'row' and the row above
     * is 'up' (up == NO means no row above). */
    static bool ready = false;
    static uint64_t trans[NO + 1][ROWS];
    if (!ready) {
        for (int up = 0; up <= NO; ++up) {
            bool has_up = (up != NO);
            uint64_t up_mask = has_up ? (uint64_t)up : 0;
            for (int row = 0; row < ROWS; ++row) {
                uint64_t row_mask = (uint64_t)row;
                uint64_t mask = 0;
                for (int down = 0; down < ROWS; ++down) {
                    uint64_t down_mask = (uint64_t)down;
                    if (row_is_valid(up_mask, row_mask, down_mask,
                                     has_up, true))
                        mask |= ((uint64_t)1ULL << down);
                }
                trans[up][row] = mask;
            }
        }
        ready = true;
    }

    /* Dynamic programming tables.
     * dp[up][row] = number of ways to build the processed rows
     * ending with previous row 'up' and current row 'row'. */
    seq_t dp[NO + 1][ROWS];
    seq_t ndp[NO + 1][ROWS];
    for (int i = 0; i <= NO; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp[i][j] = 0;

    /* First row: no row above, upper‑left cell (col 0) must be 0. */
    for (int mask = 0; mask < ROWS; ++mask)
        if ((mask & 1) == 0)   /* leftmost bit is 0 */
            dp[NO][mask] = 1;

    /* Process subsequent rows. */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i <= NO; ++i)
            for (int j = 0; j < ROWS; ++j)
                ndp[i][j] = 0;

        for (int up = 0; up <= NO; ++up) {
            for (int row = 0; row < ROWS; ++row) {
                seq_t val = dp[up][row];
                if (val == 0) continue;
                uint64_t allow = trans[up][row];
                while (allow) {
                    int down = __builtin_ctzll(allow);
                    allow &= allow - 1;          /* clear lowest set bit */
                    ndp[row][down] += val;
                }
            }
        }

        /* Copy ndp to dp for next iteration. */
        for (int i = 0; i <= NO; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Final row: no row below. Must check validity with has_down = false. */
    seq_t ans = 0;
    for (int up = 0; up <= NO; ++up) {
        bool has_up = (up != NO);
        uint64_t up_mask = has_up ? (uint64_t)up : 0;
        for (int row = 0; row < ROWS; ++row) {
            seq_t val = dp[up][row];
            if (val == 0) continue;
            if (row_is_valid(up_mask, (uint64_t)row, 0ULL, has_up, false))
                ans += val;
        }
    }
    return ans;
}
