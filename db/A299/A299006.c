/*
 * A299006  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 0/1 n×6 arrays in which each cell has 0,1,2,3,5 or 6
 *            king‑move neighbours equal to its value; the upper‑left element
 *            is 0. The computation is performed by dynamic programming over
 *            the rows, using a state of two consecutive rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define COLS 6
#define ROWS 6
#define ROWMASK (1U << COLS)

static bool allowed[9] = {
    true,  /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    true,  /* 5 */
    true,  /* 6 */
    false, /* 7 */
    false  /* 8 */
};

/* Test whether a row is valid when its above and below neighbours
   are given.  has_above/below indicate if the corresponding row
   exists.  above_row/below_row are unused when the row does not exist. */
static inline bool row_valid(int above_row, int current_row, int below_row,
                             bool has_above, bool has_below)
{
    for (int col = 0; col < COLS; ++col) {
        int val   = (current_row >> col) & 1;
        int cnt   = 0;

        /* left neighbor */
        if (col > 0 && ((current_row >> (col - 1)) & 1) == val) cnt++;
        /* right neighbor */
        if (col < COLS - 1 && ((current_row >> (col + 1)) & 1) == val) cnt++;

        if (has_above) {
            int val_above = (above_row >> col) & 1;
            if (val_above == val) cnt++;
            if (col > 0) {
                int diag_left = (above_row >> (col - 1)) & 1;
                if (diag_left == val) cnt++;
            }
            if (col < COLS - 1) {
                int diag_right = (above_row >> (col + 1)) & 1;
                if (diag_right == val) cnt++;
            }
        }

        if (has_below) {
            int val_below = (below_row >> col) & 1;
            if (val_below == val) cnt++;
            if (col > 0) {
                int diag_left = (below_row >> (col - 1)) & 1;
                if (diag_left == val) cnt++;
            }
            if (col < COLS - 1) {
                int diag_right = (below_row >> (col + 1)) & 1;
                if (diag_right == val) cnt++;
            }
        }

        if (!allowed[cnt]) return false;
    }
    return true;
}

/* Precomputed tables */
static bool topValid[ROWMASK][ROWMASK];
static bool bottomValid[ROWMASK][ROWMASK];
static uint64_t nextMask[ROWMASK][ROWMASK];

/* Initialize helper tables */
static void init_tables(void)
{
    for (int a = 0; a < ROWMASK; ++a) {
        for (int b = 0; b < ROWMASK; ++b) {
            /* top row validation : no above, below = b */
            topValid[a][b] = row_valid(0, a, b, false, true);
            /* bottom row validation : above = a, no below */
            bottomValid[a][b] = row_valid(a, b, 0, true, false);
        }
    }
    for (int a = 0; a < ROWMASK; ++a) {
        for (int b = 0; b < ROWMASK; ++b) {
            uint64_t mask = 0;
            for (int c = 0; c < ROWMASK; ++c) {
                if (row_valid(a, b, c, true, true))
                    mask |= 1ULL << c;
            }
            nextMask[a][b] = mask;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    static bool tables_ready = false;
    if (!tables_ready) {
        init_tables();
        tables_ready = true;
    }

    /* n == 1 */
    if (n == 1) {
        /* all 6‑cell rows with first cell 0 are allowed */
        return (seq_t)32;
    }

    /* n == 2 : both rows must satisfy top and bottom constraints */
    if (n == 2) {
        seq_t result = 0;
        for (int r0 = 0; r0 < ROWMASK; ++r0) {
            if ((r0 & 1) == 0) {            /* upper‑left zero */
                for (int r1 = 0; r1 < ROWMASK; ++r1) {
                    if (topValid[r0][r1] && bottomValid[r0][r1])
                        result += 1;
                }
            }
        }
        return result;
    }

    /* n >= 3 : DP over pairs of rows */
    /* dp[a*ROWMASK + b] = number of ways up to row index (i-1) where
       row a is the previous‑previous row and row b is the previous row
       (both previous rows have been validated). */
    static seq_t dp[ROWMASK * ROWMASK];
    static seq_t dp_next[ROWMASK * ROWMASK];
    for (idx_t i = 0; i < ROWMASK * ROWMASK; ++i)
        dp[i] = 0;

    /* initialise with two rows r0, r1; r0 is top row (validated by topValid) */
    for (int r0 = 0; r0 < ROWMASK; ++r0) {
        if ((r0 & 1) == 0) {        /* upper‑left zero */
            for (int r1 = 0; r1 < ROWMASK; ++r1) {
                if (topValid[r0][r1])
                    dp[r0 * ROWMASK + r1] += 1;
            }
        }
    }

    /* process remaining rows at indices 2 .. n-1 */
    idx_t steps = n - 2;                /* number of transitions to perform */
    for (idx_t step = 0; step < steps; ++step) {
        for (idx_t i = 0; i < ROWMASK * ROWMASK; ++i)
            dp_next[i] = 0;

        for (idx_t idx = 0; idx < ROWMASK * ROWMASK; ++idx) {
            seq_t cur = dp[idx];
            if (cur == 0) continue;
            int prev_prev = idx / ROWMASK;   /* a */
            int prev     = idx % ROWMASK;    /* b */
            uint64_t mask = nextMask[prev_prev][prev];
            while (mask) {
                int nxt = __builtin_ctzll(mask);
                mask &= mask - 1;
                idx_t dest = ((idx_t)prev << 6) | (idx_t)nxt;
                dp_next[dest] += cur;
            }
        }
        /* swap dp and dp_next */
        for (idx_t i = 0; i < ROWMASK * ROWMASK; ++i)
            dp[i] = dp_next[i];
    }

    /* final step: validate the last row with its above neighbour */
    seq_t result = 0;
    for (idx_t idx = 0; idx < ROWMASK * ROWMASK; ++idx) {
        seq_t cur = dp[idx];
        if (cur == 0) continue;
        int prev = idx / ROWMASK;   /* a = row_{n-2} */
        int last = idx % ROWMASK;   /* b = row_{n-1} */
        if (bottomValid[prev][last])
            result += cur;
    }
    return result;
}
