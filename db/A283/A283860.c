/*
 * A283860  Number of 4Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays in which every 1 has at most
 *            three of its six horizontal/diagonal neighbours (left/right,
 *            northwest, northeast, southwest, southeast) equal to 1. The
 *            algorithm uses a transfer‑matrix Dynamic Programming with a
 *            state of the last two columns
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool col_valid(uint8_t curr, uint8_t left, uint8_t right)
{
    /* Check the 1‑cells in the current column.  For each 1 determine how
       many of its six neighbours in the same row or adjacent rows are also 1.
       The neighbours include the cell from the previous column (left),
       the next column (right), and the four diagonal cells derived from them.
       Cells outside the 4×… array are treated as 0. */
    for (uint8_t r = 0; r < 4; ++r) {
        if ((curr >> r) & 1u) {
            uint8_t cnt = 0;
            /* horizontal neighbours */
            cnt += (left  >> r) & 1u;
            cnt += (right >> r) & 1u;
            /* diagonal neighbours from left column */
            if (r > 0)   cnt += (left  >> (r - 1)) & 1u;
            if (r < 3)   cnt += (left  >> (r + 1)) & 1u;
            /* diagonal neighbours from right column */
            if (r > 0)   cnt += (right >> (r - 1)) & 1u;
            if (r < 3)   cnt += (right >> (r + 1)) & 1u;
            if (cnt > 3u) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but handle safely */
    if (n == 1) return (seq_t)16;     /* all single columns are allowed */

    /* DP state: dp[prev][curr] counts ways for the last two columns
       (prev = column i-1, curr = column i).  Each column is a 4‑bit mask. */
    seq_t dpPrev[16][16] = {0};
    seq_t dpNext[16][16] = {0};

    /* Initialise with the first two columns.  The first column is checked
       against a non‑existent left column (treated as all zeros). */
    for (int col0 = 0; col0 < 16; ++col0)
        for (int col1 = 0; col1 < 16; ++col1)
            if (col_valid((uint8_t)col0, 0u, (uint8_t)col1))
                dpPrev[col0][col1] = (seq_t)1;

    /* Process columns 2 … n-1 (0‑based indexing).  After adding a new column,
       the middle column (the current 'curr' from the previous step) can be
       fully validated. */
    seq_t (*cur)[16]  = dpPrev;
    seq_t (*next)[16] = dpNext;

    for (idx_t step = 2; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = cur[prev][curr];
                if (!ways) continue;
                for (int nextcol = 0; nextcol < 16; ++nextcol) {
                    if (col_valid((uint8_t)curr,
                                  (uint8_t)prev,
                                  (uint8_t)nextcol)) {
                        next[curr][nextcol] += ways;
                    }
                }
            }
        }
        /* swap cur and next */
        seq_t (*tmp)[16] = cur;
        cur  = next;
        next = tmp;
    }

    /* After all columns are processed, validate the last column against a
       non‑existent right column (treated as all zeros). */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
            if (col_valid((uint8_t)curr,
                          (uint8_t)prev,
                          0u))
                total += cur[prev][curr];

    return total;
}
