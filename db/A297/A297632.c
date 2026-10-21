/*
 * A297632  Number of nX3 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A297632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays in which each 1 has 0, 2, or
 *            3 horizontal or diagonal neighbours (left/right,
 *            up-left/up-right, down-left/down-right). This is computed by
 *            dynamic programming over the rows
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline bool bit(uint8_t r, int c) { return (r >> c) & 1u; }

/* Validate a top row using the row below it.  Only left/right
   in the top row and down‑left/down‑right into the row below
   contribute to neighbour counts. */
static inline bool valid_first_row(uint8_t top, uint8_t below)
{
    for (int j = 0; j < 3; ++j)
        if (bit(top, j)) {
            int cnt = 0;
            if (j > 0 && bit(top, j - 1)) ++cnt;          /* left */
            if (j < 2 && bit(top, j + 1)) ++cnt;          /* right */
            if (j > 0 && bit(below, j - 1)) ++cnt;        /* down‑left */
            if (j < 2 && bit(below, j + 1)) ++cnt;        /* down‑right */
            if (cnt != 0 && cnt != 2 && cnt != 3) return false;
        }
    return true;
}

/* Validate a middle row knowing both the row above and below it. */
static inline bool valid_middle_row(uint8_t above, uint8_t cur, uint8_t below)
{
    for (int j = 0; j < 3; ++j)
        if (bit(cur, j)) {
            int cnt = 0;
            if (j > 0 && bit(cur, j - 1)) ++cnt;          /* left */
            if (j < 2 && bit(cur, j + 1)) ++cnt;          /* right */
            if (j > 0 && bit(above, j - 1)) ++cnt;       /* up‑left */
            if (j < 2 && bit(above, j + 1)) ++cnt;       /* up‑right */
            if (j > 0 && bit(below, j - 1)) ++cnt;       /* down‑left */
            if (j < 2 && bit(below, j + 1)) ++cnt;       /* down‑right */
            if (cnt != 0 && cnt != 2 && cnt != 3) return false;
        }
    return true;
}

/* Validate a last row using the row above it.  No row below exists. */
static inline bool valid_last_row(uint8_t above, uint8_t cur)
{
    for (int j = 0; j < 3; ++j)
        if (bit(cur, j)) {
            int cnt = 0;
            if (j > 0 && bit(cur, j - 1)) ++cnt;          /* left */
            if (j < 2 && bit(cur, j + 1)) ++cnt;          /* right */
            if (j > 0 && bit(above, j - 1)) ++cnt;       /* up‑left */
            if (j < 2 && bit(above, j + 1)) ++cnt;       /* up‑right */
            if (cnt != 0 && cnt != 2 && cnt != 3) return false;
        }
    return true;
}

/* Validate a single row with no rows above or below (used for n = 1). */
static inline bool valid_single_row(uint8_t cur)
{
    for (int j = 0; j < 3; ++j)
        if (bit(cur, j)) {
            int cnt = 0;
            if (j > 0 && bit(cur, j - 1)) ++cnt;          /* left */
            if (j < 2 && bit(cur, j + 1)) ++cnt;          /* right */
            if (cnt != 0 && cnt != 2) return false;       /* only 0 or 2 possible */
        }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe to return 0 */

    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t r = 0; r < 8; ++r)
            if (valid_single_row(r)) ans += 1;
        return ans;
    }

    /* dp[prev][cur] counts arrays up to current row where
       prev is the second‑to‑last row, cur is the last row,
       and all rows up to prev have been validated. */
    seq_t dp[8][8] = {0};
    seq_t newdp[8][8] = {0};

    /* Initialise with the first two rows.  The top row must be
       valid when only its below neighbours are considered. */
    for (int top = 0; top < 8; ++top)
        for (int bottom = 0; bottom < 8; ++bottom)
            if (valid_first_row((uint8_t)top, (uint8_t)bottom))
                dp[top][bottom] = 1;

    /* Process remaining rows 3 .. n.  On each step we validate
       the middle row using its upper and lower neighbours. */
    for (idx_t row = 3; row <= n; ++row) {
        /* zero newdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (int next = 0; next < 8; ++next)
                    if (valid_middle_row((uint8_t)prev, (uint8_t)cur, (uint8_t)next))
                        newdp[cur][next] += ways;
            }
        }

        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* After all rows, validate the last row (no below neighbours). */
    seq_t result = 0;
    for (int prev = 0; prev < 8; ++prev)
        for (int last = 0; last < 8; ++last) {
            seq_t ways = dp[prev][last];
            if (!ways) continue;
            if (valid_last_row((uint8_t)prev, (uint8_t)last))
                result += ways;
        }

    return result;
}
