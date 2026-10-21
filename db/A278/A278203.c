/*
 * A278203  Number of n X 3 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) (1,-1) (1,0) or (1,1), with upper left element zero.
 * https://oeis.org/A278203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP counting n×3 binary arrays with constraints that each cell
 *            has at least one equal and one unequal neighbour among the
 *            existing neighbours; the upper‑left element is fixed to 0. The
 *            OEIS recurrence is
 *            a(n)=6a(n-1)+7a(n-2)+11a(n-3)+11a(n-4)-9a(n-5)+3a(n-7)-2a(n-8)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int NONE = 8;               /* sentinel for a missing row */

/* Check whether a row mask `curr` satisfies the equal/unequal neighbour condition
   when the row above is `above` (NONE if missing) and the row below is `below`
   (NONE if missing).  Masks are 3‑bit integers (0..7). */
static inline bool row_valid(int above, int curr, int below)
{
    for (int col = 0; col < 3; ++col) {
        int val = (curr >> col) & 1;
        bool has_equal = false, has_notequal = false;

        /* same row neighbours */
        if (col > 0) {
            int nb = (curr >> (col - 1)) & 1;
            if (val == nb) has_equal = true; else has_notequal = true;
        }
        if (col < 2) {
            int nb = (curr >> (col + 1)) & 1;
            if (val == nb) has_equal = true; else has_notequal = true;
        }

        /* above neighbours */
        if (above != NONE) {
            if (col > 0) {
                int nb = (above >> (col - 1)) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
            {
                int nb = (above >> col) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
            if (col < 2) {
                int nb = (above >> (col + 1)) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
        }

        /* below neighbours */
        if (below != NONE) {
            if (col > 0) {
                int nb = (below >> (col - 1)) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
            {
                int nb = (below >> col) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
            if (col < 2) {
                int nb = (below >> (col + 1)) & 1;
                if (val == nb) has_equal = true; else has_notequal = true;
            }
        }

        if (!(has_equal && has_notequal))
            return false; /* this cell fails the requirement */
    }
    return true; /* all cells satisfy */
}

seq_t a(idx_t n)
{
    /* dp[a][b] = number of ways to fill the first i rows where
       the last two rows are masks a (row i-2) and b (row i-1) */
    static seq_t dp[9][8];
    static seq_t ndp[9][8];
    for (int a = 0; a < 9; ++a)
        for (int b = 0; b < 8; ++b)
            dp[a][b] = 0;

    /* first row, upper‑left element must be 0 (bit 0 = 0) */
    for (int mask = 0; mask < 8; ++mask)
        if ((mask & 1) == 0) /* bit0 = 0 */
            dp[NONE][mask] = 1;

    /* iterate over remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        for (int a = 0; a < 9; ++a)
            for (int b = 0; b < 8; ++b)
                ndp[a][b] = 0;

        for (int a = 0; a < 9; ++a) {
            for (int b = 0; b < 8; ++b) {
                seq_t curcnt = dp[a][b];
                if (curcnt == 0) continue;
                for (int c = 0; c < 8; ++c) {
                    if (!row_valid(a, b, c)) continue;
                    ndp[b][c] += curcnt;
                }
            }
        }

        /* copy back to dp */
        for (int a = 0; a < 9; ++a)
            for (int b = 0; b < 8; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* now we have processed all n rows.  Finalise last row (mask b)
       with above a and no below neighbours. */
    seq_t result = 0;
    for (int a = 0; a < 9; ++a) {
        for (int b = 0; b < 8; ++b) {
            seq_t curcnt = dp[a][b];
            if (curcnt == 0) continue;
            if (row_valid(a, b, NONE))
                result += curcnt;
        }
    }
    return result;
}
