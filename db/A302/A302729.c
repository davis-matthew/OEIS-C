/*
 * A302729  Number of 4Xn 0..1 arrays with every element equal to 1, 2, 4 or 6
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function computes the sequence A302729 using a *
 *            transfer-matrix technique. It keeps track of the last two
 *            columns * of a 4×n binary array and validates each column as
 *            soon as its * left and right neighbors are known. The upper‑left
 *            element * (row 0) of the first column is forced to 0
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define PREV_NONE 16             /* special value for absent left column */

/* Test whether a column satisfies the neighbour‑count condition
 * (numbers of equal neighbours must be 1, 2, 4 or 6) given the
 * existence and values of the left and right neighbouring columns. */
static inline bool column_valid(uint8_t col, uint8_t left, bool left_ok,
                                uint8_t right, bool right_ok)
{
    for (int r = 0; r < 4; ++r) {
        uint8_t v = (col >> r) & 1;
        int cnt = 0;
        /* left neighbours */
        if (left_ok) {
            if (((left >> r) & 1) == v)               ++cnt; /* same row */
            if (r > 0   && (((left >> (r-1)) & 1) == v)) ++cnt; /* up-left */
            if (r < 3   && (((left >> (r+1)) & 1) == v)) ++cnt; /* down-left */
        }
        /* right neighbours */
        if (right_ok) {
            if (((right >> r) & 1) == v)               ++cnt; /* same row */
            if (r > 0   && (((right >> (r-1)) & 1) == v)) ++cnt; /* up-right */
            if (r < 3   && (((right >> (r+1)) & 1) == v)) ++cnt; /* down-right */
        }
        if (!(cnt == 1 || cnt == 2 || cnt == 4 || cnt == 6))
            return false;
    }
    return true;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    /* dp[prev][curr] – number of ways to obtain the pattern
     * up to the current column, where the two most recent
     * columns are prev and curr; the column 'prev' has not yet
     * been fully validated (its right neighbour is still unknown). */
    seq_t dp[PREV_NONE + 1][16] = { 0 };
    seq_t next[PREV_NONE + 1][16] = { 0 };

    /* initialise with all possibilities for the first column,
     * respecting the fixed upper‑left element (row 0) = 0. */
    for (uint8_t col = 0; col < 16; ++col) {
        if (col & 1)                         /* row 0 must be 0 */
            continue;
        dp[PREV_NONE][col] = 1;
    }

    /* add columns 2 … n */
    for (idx_t cur = 2; cur <= n; ++cur) {
        /* clear next dp array */
        for (int i = 0; i <= PREV_NONE; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = 0;

        for (int prev = 0; prev <= PREV_NONE; ++prev) {
            bool prev_ok = (prev != PREV_NONE);
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways)
                    continue;
                for (uint8_t nxt = 0; nxt < 16; ++nxt) {
                    if (!column_valid(curr, prev_ok ? (uint8_t)prev : 0,
                                      prev_ok, nxt, true))
                        continue;
                    next[curr][nxt] += ways;
                }
            }
        }
        /* move to the next step */
        for (int i = 0; i <= PREV_NONE; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = next[i][j];
    }

    /* final step – validate the last column (right neighbour absent) */
    seq_t result = 0;
    for (int prev = 0; prev <= PREV_NONE; ++prev) {
        bool prev_ok = (prev != PREV_NONE);
        for (int curr = 0; curr < 16; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways)
                continue;
            if (!column_valid(curr, prev_ok ? (uint8_t)prev : 0,
                              prev_ok, 0, false))
                continue;
            result += ways;
        }
    }

    return result;
}
