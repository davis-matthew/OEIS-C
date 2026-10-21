/*
 * A299390  Number of nX5 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          6 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with upper‑left cell 0 * and,
 *            for every entry, the number of king‑move adjacent entries equal
 *            * to it belonging to {0,1,3,4,6,8}. This is computed by dynamic
 *            * programming over all 32 possible row patterns
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define ROWS 32
#define COLS 5

/* Allowed numbers of equal neighbours (0‑based index). */
static const unsigned char allowed[9] = {
    1,  /* 0 */
    1,  /* 1 */
    0,  /* 2 */
    1,  /* 3 */
    1,  /* 4 */
    0,  /* 5 */
    1,  /* 6 */
    0,  /* 7 */
    1   /* 8 */
};

/* Check a row that has no neighbours above or below (single row). */
static int single_row_ok(unsigned mask)
{
    for (int j = 0; j < COLS; ++j) {
        int v = (mask >> j) & 1;
        int c = 0;
        if (j > 0 && (((mask >> (j-1)) & 1) == v)) ++c;
        if (j + 1 < COLS && (((mask >> (j+1)) & 1) == v)) ++c;
        if (!allowed[c]) return 0;
    }
    return 1;
}

/* Check a row given optional rows above and below. */
static int row_ok(unsigned cur, unsigned up, unsigned down,
                  int has_up, int has_down)
{
    for (int j = 0; j < COLS; ++j) {
        int v = (cur >> j) & 1;
        int c = 0;
        /* Same row neighbours */
        if (j > 0 && (((cur >> (j-1)) & 1) == v)) ++c;
        if (j + 1 < COLS && (((cur >> (j+1)) & 1) == v)) ++c;
        /* Neighbours above */
        if (has_up) {
            if (j > 0 && (((up >> (j-1)) & 1) == v)) ++c;
            if ((((up >> j) & 1) == v)) ++c;
            if (j + 1 < COLS && ((((up >> (j+1)) & 1) == v))) ++c;
        }
        /* Neighbours below */
        if (has_down) {
            if (j > 0 && ((((down >> (j-1)) & 1) == v))) ++c;
            if ((((down >> j) & 1) == v)) ++c;
            if (j + 1 < COLS && ((((down >> (j+1)) & 1) == v))) ++c;
        }
        if (!allowed[c]) return 0;
    }
    return 1;
}

/* Tables for fast transition checks. */
static int init_done = 0;
static int top_valid[ROWS][ROWS];
static int bottom_valid[ROWS][ROWS];
static unsigned char next_cnt[ROWS][ROWS];
static unsigned char next_list[ROWS][ROWS][ROWS];

/* Pre‑compute validity tables once. */
static void init_tables(void)
{
    if (init_done) return;

    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            /* Top row: no above neighbours, second row below */
            top_valid[a][b] = row_ok(a, 0, b, 0, 1);
            /* Bottom row: previous row above, no below neighbours */
            bottom_valid[a][b] = row_ok(b, a, 0, 1, 0);

            unsigned char cnt = 0;
            for (int c = 0; c < ROWS; ++c) {
                if (row_ok(b, a, c, 1, 1)) {
                    next_list[a][b][cnt++] = (unsigned char)c;
                }
            }
            next_cnt[a][b] = cnt;
        }
    }
    init_done = 1;
}

seq_t a(idx_t n)
{
    if (!init_done) init_tables();

    /* Special case: a single row. */
    if (n == 1) {
        seq_t total = 0;
        for (unsigned mask = 0; mask < ROWS; ++mask) {
            if ((mask & 1) == 0 && single_row_ok(mask)) {
                total += 1;
            }
        }
        return total;
    }

    /* DP: dp[prev][curr] = number of ways for processed rows
     * whose last two rows are (prev,curr). */
    static seq_t dp[ROWS][ROWS];
    static seq_t dp_new[ROWS][ROWS];

    /* Clear dp */
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp[i][j] = 0;

    /* Initialise with first two rows */
    for (unsigned top = 0; top < ROWS; ++top) {
        if ((top & 1) != 0) continue;           /* upper‑left cell must be 0 */
        for (unsigned second = 0; second < ROWS; ++second) {
            if (top_valid[top][second]) {
                dp[top][second] += 1;
            }
        }
    }

    /* Add rows 3..n */
    for (idx_t step = 2; step < n; ++step) {   /* step is index of next row to add */
        /* Clear dp_new */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp_new[i][j] = 0;

        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                unsigned char cnt = next_cnt[prev][curr];
                unsigned char *lst = next_list[prev][curr];
                for (unsigned k = 0; k < cnt; ++k) {
                    unsigned nxt = lst[k];
                    dp_new[curr][nxt] += val;
                }
            }
        }
        /* Swap dp and dp_new */
        for (int i = 0; i < ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp[i][j] = dp_new[i][j];
    }

    /* Finalise: check bottom row validity */
    seq_t result = 0;
    for (int prev = 0; prev < ROWS; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            seq_t val = dp[prev][curr];
            if (!val) continue;
            if (bottom_valid[prev][curr]) {
                result += val;
            }
        }
    }
    return result;
}
