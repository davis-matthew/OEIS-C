/*
 * A278152  Number of nX3 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) (1,-1) or (1,0), with upper left element zero.
 * https://oeis.org/A278152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Number of n×3 0/1 arrays with 
 * every element equal to at least one neighbour and not equal to at least one neighbour
 * among offsets (-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0).
 * Upper‑left element must be zero. 
 * Computed via dynamic programming over rows.
 */

#include <stddef.h>
#include <stdbool.h>

/*---------------------------------------------------------------*/
/*  Helper: check if a row satisfies the constraints given the
 *  previous, current and next rows.                                     */
static bool row_ok(int prev, int cur, int next,
                   bool top_exists, bool bottom_exists)
{
    /* iterate over each of the 3 columns */
    for (int j = 0; j < 3; ++j) {
        int cell = (cur >> j) & 1;
        bool equal = false;
        bool notequal = false;

        /* same row neighbours */
        if (j - 1 >= 0) {
            int nb = (cur >> (j - 1)) & 1;
            if (nb == cell) equal = true; else notequal = true;
        }
        if (j + 1 < 3) {
            int nb = (cur >> (j + 1)) & 1;
            if (nb == cell) equal = true; else notequal = true;
        }

        /* upper neighbours */
        if (top_exists) {
            if (j - 1 >= 0) {
                int nb = (prev >> (j - 1)) & 1;
                if (nb == cell) equal = true; else notequal = true;
            }
            {
                int nb = (prev >> j) & 1;
                if (nb == cell) equal = true; else notequal = true;
            }
            if (j + 1 < 3) {
                int nb = (prev >> (j + 1)) & 1;
                if (nb == cell) equal = true; else notequal = true;
            }
        }

        /* lower neighbours */
        if (bottom_exists) {
            if (j - 1 >= 0) {
                int nb = (next >> (j - 1)) & 1;
                if (nb == cell) equal = true; else notequal = true;
            }
            {
                int nb = (next >> j) & 1;
                if (nb == cell) equal = true; else notequal = true;
            }
        }

        if (!(equal && notequal))
            return false;
    }
    return true;
}

/*---------------------------------------------------------------*/
/*  Pre‑computed table of the validity of a triple (prev,cur,next)
 *  for the two possible rows preceding or following the current row. */
static bool ok_table[2][2][8][8][8];
static bool ok_table_ready = false;

static void init_ok_table(void)
{
    for (int ttop = 0; ttop < 2; ++ttop) {
        for (int tbottom = 0; tbottom < 2; ++tbottom) {
            for (int p = 0; p < 8; ++p) {
                for (int c = 0; c < 8; ++c) {
                    for (int n = 0; n < 8; ++n) {
                        ok_table[ttop][tbottom][p][c][n] =
                            row_ok(p, c, n, ttop, tbottom);
                    }
                }
            }
        }
    }
    ok_table_ready = true;
}

/*---------------------------------------------------------------*/
/*  Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1, return 0 */
        return 0;

    if (!ok_table_ready)
        init_ok_table();

    const int PATTERNS = 1 << 3;          /* 8 patterns for a row */
    const int PREV_NONE = PATTERNS;       /* 8 indicates 'no previous row' */
    const int STATES = (PATTERNS + 1) * PATTERNS;  /* 9*8 = 72 */

    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];
    for (int i = 0; i < STATES; ++i) {
        dp_cur[i] = 0;
        dp_next[i] = 0;
    }

    /* initialise first row (row 0) */
    for (int cur = 0; cur < PATTERNS; ++cur) {
        if ((cur & 1) == 0) {            /* upper‑left element must be 0 */
            dp_cur[PREV_NONE * PATTERNS + cur] = 1;
        }
    }

    /* process rows 1 .. n-1 */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < STATES; ++i)
            dp_next[i] = 0;

        for (int prev = 0; prev <= PREV_NONE; ++prev) {
            bool top_exists = (prev != PREV_NONE);
            int prev_idx = top_exists ? prev : 0;   /* index for table */

            for (int cur = 0; cur < PATTERNS; ++cur) {
                seq_t ways = dp_cur[prev * PATTERNS + cur];
                if (ways == 0) continue;
                for (int next = 0; next < PATTERNS; ++next) {
                    if (ok_table[ top_exists ? 1 : 0 ][1][prev_idx][cur][next]) {
                        int index = cur * PATTERNS + next;
                        dp_next[index] += ways;
                    }
                }
            }
        }

        /* swap dp arrays */
        for (int i = 0; i < STATES; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* final row (row n-1) has no bottom neighbours */
    seq_t result = 0;
    for (int prev = 0; prev <= PREV_NONE; ++prev) {
        bool top_exists = (prev != PREV_NONE);
        int prev_idx = top_exists ? prev : 0;   /* index for table */

        for (int cur = 0; cur < PATTERNS; ++cur) {
            seq_t ways = dp_cur[prev * PATTERNS + cur];
            if (ways == 0) continue;
            if (ok_table[ top_exists ? 1 : 0 ][0][prev_idx][cur][0])
                result += ways;
        }
    }

    return result;
}
