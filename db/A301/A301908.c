/*
 * A301908  Number of 4Xn 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses DP over pairs of adjacent columns.
   Each state (p,c) represents the last two columns of a partial
   4×k array.  All constraints for columns up to the first of the
   pair are already satisfied.  When adding a new column `q`,
   the constraint for column `c` is checked with neighbors `p` and `q`.
   The upper‑left element (row 0, col 0) is forced to 0. */
#include <stdbool.h>
#include <stddef.h>

static const int NO_COL = 16;          /* sentinel for a missing column */

/* Check whether column 'mid' satisfies the equal‑neighbor count
   rule, given left neighbour 'left' and right neighbour 'right'.
   Missing neighbours are indicated by NO_COL. */
static bool valid_transition(int left, int mid, int right)
{
    for (int r = 0; r < 4; ++r) {
        unsigned mid_bit = (mid >> r) & 1U;
        unsigned count   = 0;

        /* left neighbour (same row) */
        if (left != NO_COL) {
            if (mid_bit == ((left >> r) & 1U))
                ++count;
        }

        /* right neighbour (same row) */
        if (right != NO_COL) {
            if (mid_bit == ((right >> r) & 1U))
                ++count;
        }

        /* up‑right neighbour (row‑1, next column) */
        if (right != NO_COL && r > 0) {
            if (mid_bit == ((right >> (r - 1)) & 1U))
                ++count;
        }

        /* down‑left neighbour (row+1, previous column) */
        if (left != NO_COL && r < 3) {
            if (mid_bit == ((left >> (r + 1)) & 1U))
                ++count;
        }

        /* count must not be 1 or 4 */
        if (count == 1 || count == 4)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* DP arrays: dp[p][c] = number of ways for the last two columns
       (p,c) after processing some prefix. */
    seq_t dp[16][16];
    seq_t ndp[16][16];

    /* Initialize all entries to zero */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* Handle the first two columns */
    for (int col0 = 0; col0 < 16; ++col0) {
        if (col0 & 1) continue;          /* upper‑left must be 0 */
        for (int col1 = 0; col1 < 16; ++col1) {
            if (valid_transition(NO_COL, col0, col1))
                dp[col0][col1] = dp[col0][col1] + 1;   /* each pair is counted once */
        }
    }

    if (n == 1) {
        /* Only one column: check constraints with no right neighbour */
        seq_t total = 0;
        for (int col = 0; col < 16; ++col) {
            if (col & 1) continue;
            if (valid_transition(NO_COL, col, NO_COL))
                total = total + 1;
        }
        return total;
    }

    /* Process third column onwards */
    for (idx_t step = 2; step < n; ++step) {
        /* Clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int left = 0; left < 16; ++left) {
            for (int mid = 0; mid < 16; ++mid) {
                seq_t ways = dp[left][mid];
                if (ways == 0) continue;
                for (int right = 0; right < 16; ++right) {
                    if (!valid_transition(left, mid, right))
                        continue;
                    ndp[mid][right] = ndp[mid][right] + ways;
                }
            }
        }
        /* Copy ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum over final states, checking constraint for the last column */
    seq_t answer = 0;
    for (int left = 0; left < 16; ++left) {
        for (int mid = 0; mid < 16; ++mid) {
            if (!valid_transition(left, mid, NO_COL))
                continue;
            answer = answer + dp[left][mid];
        }
    }
    return answer;
}
