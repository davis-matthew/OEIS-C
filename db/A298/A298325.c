/*
 * A298325  Number of nX5 0..1 arrays with every element equal to 0, 2, 3, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: count 5×n binary matrices whose every cell has exactly
 * 0,2,3,4 or 6 equal king‑adjacent neighbours, with the upper‑left
 * element fixed to 0.  Uses dynamic programming over the rows. */

#include <stdint.h>
#include <stdbool.h>

#define ROWS 32                     /* 5‑bit patterns: 0 … 31 */
static const int ALLOWED[9] = {     /* allowed neighbour counts */
    1, /* 0 */
    0, /* 1 */
    1, /* 2 */
    1, /* 3 */
    1, /* 4 */
    0, /* 5 */
    1, /* 6 */
    0, /* 7 */
    0  /* 8 */
};

static const idx_t NO_ROW = (idx_t)-1;

/* test whether a row configuration is locally valid, given
 * the rows above and below (NO_ROW means no row exists) */
static bool is_valid(idx_t prev, idx_t curr, idx_t next)
{
    for (int c = 0; c < 5; ++c) {
        int bit   = (curr >> c) & 1;
        int count = 0;

        /* same row left/right */
        if (c > 0   && ((curr >> (c-1)) & 1) == bit) ++count;
        if (c < 4   && ((curr >> (c+1)) & 1) == bit) ++count;

        /* upper neighbours */
        if (prev != NO_ROW) {
            if (((prev >> c) & 1) == bit) ++count;
            if (c > 0   && ((prev >> (c-1)) & 1) == bit) ++count;
            if (c < 4   && ((prev >> (c+1)) & 1) == bit) ++count;
        }

        /* lower neighbours */
        if (next != NO_ROW) {
            if (((next >> c) & 1) == bit) ++count;
            if (c > 0   && ((next >> (c-1)) & 1) == bit) ++count;
            if (c < 4   && ((next >> (c+1)) & 1) == bit) ++count;
        }

        if (!ALLOWED[count])
            return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* tables of pre‑computed validity */
    static bool firstValid[ROWS][ROWS];
    static bool middleValid[ROWS][ROWS][ROWS];
    static bool lastValid[ROWS][ROWS];
    static bool validSingle[ROWS];
    static bool init = false;

    if (!init) {
        /* fill tables once */
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                firstValid[prev][curr] = is_valid(NO_ROW,
                                                 (idx_t)prev,
                                                 (idx_t)curr);
                lastValid[prev][curr]  = is_valid((idx_t)prev,
                                                 (idx_t)curr,
                                                 NO_ROW);
                for (int next = 0; next < ROWS; ++next) {
                    middleValid[prev][curr][next] =
                        is_valid((idx_t)prev,
                                 (idx_t)curr,
                                 (idx_t)next);
                }
            }
        }
        for (int curr = 0; curr < ROWS; ++curr) {
            validSingle[curr] = is_valid(NO_ROW,
                                         (idx_t)curr,
                                         NO_ROW);
        }
        init = true;
    }

    /* special case n == 1: only one row with no neighbours */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < ROWS; ++row) {
            if ((row & 1) == 0 && validSingle[row])
                total += (seq_t)1;
        }
        return total;
    }

    /* DP tables: dp[prev][curr] */
    static seq_t dp[ROWS][ROWS];
    static seq_t ndp[ROWS][ROWS];

    /* reset dp */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j) {
            dp[i][j] = ndp[i][j] = (seq_t)0;
        }

    /* initialise first two rows */
    for (int prev = 0; prev < ROWS; ++prev) {
        /* leftmost cell of first row must be 0 */
        if ((prev & 1) == 0) {
            for (int curr = 0; curr < ROWS; ++curr) {
                if (firstValid[prev][curr])
                    dp[prev][curr] = (seq_t)1;
            }
        }
    }

    /* add remaining rows */
    for (int step = 2; step < n; ++step) {
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                ndp[i][j] = (seq_t)0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t curval = dp[prev][curr];
                if (curval == (seq_t)0) continue;
                for (int next = 0; next < ROWS; ++next) {
                    if (middleValid[prev][curr][next]) {
                        ndp[curr][next] += curval;
                    }
                }
            }
        }

        /* swap dp and ndp for next iteration */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* sum over valid final states */
    seq_t total = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int last = 0; last < ROWS; ++last) {
            if (dp[prev][last] != (seq_t)0 && lastValid[prev][last]) {
                total += dp[prev][last];
            }
        }
    }
    return total;
}
