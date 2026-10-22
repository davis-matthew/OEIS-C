/*
 * A303011  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A303011:
   Number of n×3 0/1 arrays with upper‑left entry 0 such that
   for every element the number of horizontally, diagonally or
   antidiagonally adjacent equal elements is in {0,1,2,4,6}.
   The algorithm uses dynamic programming over rows.
   The DP states are the last two rows; a row is considered valid
   only after both its upper and lower neighbours are known.
   Three lookup tables are pre‑computed:
     valid_no_prev[ curr ][ next ]   – curr valid with no top neighbour,
     valid_no_next[ prev ][ curr ]   – curr valid with no bottom neighbour,
     valid_with_both[ prev ][ curr ][ next ] – curr valid with both neighbours.
   Complexity: O(8³ + 8²·n) ≈ 512·n operations. */

#include <stdbool.h>
#include <stddef.h>

/* Allowed neighbor‑count values. */
static const bool allowed[7] = {
    true,  /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    false, /* 3 */
    true,  /* 4 */
    false, /* 5 */
    true   /* 6 */
};

/* Tables holding pre‑computed validity. */
static bool valid_no_prev[8][8];
static bool valid_no_next[8][8];
static bool valid_with_both[8][8][8];
static bool valid_no_adjacent[8];
static bool tables_inited = false;

/* Helper that tests validity of a row, given whether the
   neighbours above/below exist. */
static bool test_row_valid(int prev_row, int curr_row, int next_row,
                           bool has_prev, bool has_next)
{
    for (int col = 0; col < 3; ++col) {
        int c = (curr_row >> col) & 1;   /* current cell value */
        int cnt = 0;

        /* horizontal neighbours */
        if (col > 0 && ((curr_row >> (col - 1)) & 1) == c) ++cnt;
        if (col < 2 && ((curr_row >> (col + 1)) & 1) == c) ++cnt;

        /* diagonal neighbours from the row above */
        if (has_prev) {
            if (col > 0 && ((prev_row >> (col - 1)) & 1) == c) ++cnt;
            if (col < 2 && ((prev_row >> (col + 1)) & 1) == c) ++cnt;
        }

        /* diagonal neighbours from the row below */
        if (has_next) {
            if (col > 0 && ((next_row >> (col - 1)) & 1) == c) ++cnt;
            if (col < 2 && ((next_row >> (col + 1)) & 1) == c) ++cnt;
        }

        if (!allowed[cnt]) return false;
    }
    return true;
}

/* Initialise the lookup tables once. */
static void init_tables(void)
{
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            for (int next = 0; next < 8; ++next) {
                valid_with_both[prev][curr][next] =
                    test_row_valid(prev, curr, next, true, true);
            }
        }
    }

    for (int curr = 0; curr < 8; ++curr) {
        for (int next = 0; next < 8; ++next) {
            valid_no_prev[curr][next] =
                test_row_valid(/*prev*/0, curr, next, false, true);
        }
    }

    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            valid_no_next[prev][curr] =
                test_row_valid(prev, curr, /*next*/0, true, false);
        }
    }

    for (int curr = 0; curr < 8; ++curr) {
        valid_no_adjacent[curr] =
            test_row_valid(0, curr, 0, false, false);
    }

    tables_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!tables_inited) init_tables();

    if (n == 0) return (seq_t)0;          /* undefined offset */
    if (n == 1) {
        seq_t cnt = 0;
        for (int m = 0; m < 8; ++m)
            if ((m & 1) == 0 && valid_no_adjacent[m])
                cnt += (seq_t)1;
        return cnt;
    }

    /* DP arrays: cur[p][c] – number of ways after validating row p-1,
                     with last two rows (prev=p, curr=c). */
    seq_t cur[8][8] = {0};
    seq_t nxt[8][8] = {0};

    /* Initialise with first two rows. */
    for (int r1 = 0; r1 < 8; ++r1) {
        if ((r1 & 1) != 0) continue;              /* upper‑left must be 0 */
        for (int r2 = 0; r2 < 8; ++r2) {
            if (valid_no_prev[r1][r2]) {
                cur[r1][r2] = (seq_t)1;
            }
        }
    }

    if (n == 2) {
        seq_t total = 0;
        for (int r1 = 0; r1 < 8; ++r1)
            for (int r2 = 0; r2 < 8; ++r2)
                if (valid_no_next[r1][r2])
                    total += cur[r1][r2];
        return total;
    }

    /* For n >= 3. */
    for (idx_t row = 3; row <= n; ++row) {
        /* reset nxt */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                nxt[i][j] = (seq_t)0;

        /* Transition: validate current row using prev and next. */
        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                seq_t ways = cur[prev][curr];
                if (!ways) continue;
                for (int nextr = 0; nextr < 8; ++nextr) {
                    if (valid_with_both[prev][curr][nextr]) {
                        nxt[curr][nextr] += ways;
                    }
                }
            }
        }

        /* Move next → cur */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Final row must satisfy constraints with no next neighbour. */
    seq_t total = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            if (valid_no_next[prev][curr])
                total += cur[prev][curr];
        }
    }
    return total;
}
