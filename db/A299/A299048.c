/*
 * A299048  Number of nX4 0..1 arrays with every element equal to 0, 1, 3, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays where each element has
 *            0,1,3,4,5 or 6 king‑move adjacent cells equal to it, and the
 *            upper left element is zero
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

/* Helper: check if a given row satisfies the adjacency constraint,
   given its above and below neighbours (if any). */
static bool row_valid(uint8_t row,
                      bool has_above, uint8_t above,
                      bool has_below, uint8_t below)
{
    /* Allowed neighbor counts: 0,1,3,4,5,6 */
    static const bool ok[7] = {true, true, false, true, true, true, true};

    for (int c = 0; c < 4; ++c) {
        int v = (row >> c) & 1;
        int cnt = 0;

        /* same‑row neighbours */
        if (c > 0     && ((row >> (c - 1)) & 1) == v) ++cnt;
        if (c < 3     && ((row >> (c + 1)) & 1) == v) ++cnt;

        /* vertical neighbours */
        if (has_above && ((above >> c) & 1) == v)       ++cnt;
        if (has_below && ((below >> c) & 1) == v)       ++cnt;

        /* diagonal neighbours */
        if (has_above && c > 0     && ((above >> (c - 1)) & 1) == v) ++cnt;
        if (has_above && c < 3     && ((above >> (c + 1)) & 1) == v) ++cnt;
        if (has_below && c > 0     && ((below >> (c - 1)) & 1) == v) ++cnt;
        if (has_below && c < 3     && ((below >> (c + 1)) & 1) == v) ++cnt;

        if (cnt > 6 || !ok[cnt]) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1) {
        seq_t total = 0;
        for (uint8_t r = 0; r < 16; ++r) {
            if ((r & 1) == 0) { /* upper left element zero */
                if (row_valid(r, false, 0, false, 0))
                    total += (seq_t)1;
            }
        }
        return total;
    }

    /* DP tables for states (prev2, prev1), 16x16 matrix */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];
    memset(dp, 0, sizeof(dp));

    /* Initialise first two rows */
    for (uint8_t r1 = 0; r1 < 16; ++r1) {
        if ((r1 & 1) == 0) {            /* upper left element zero */
            for (uint8_t r2 = 0; r2 < 16; ++r2) {
                if (row_valid(r1, false, 0, true, r2))
                    dp[r1][r2] = (seq_t)1;
            }
        }
    }

    /* Process rows 3 .. n (if any) */
    for (idx_t row = 3; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int b = 0; b < 16; ++b) {
            for (int c = 0; c < 16; ++c) {
                seq_t cnt = dp[b][c];
                if (cnt == 0) continue;
                for (int d = 0; d < 16; ++d) {
                    if (row_valid(c, true, (uint8_t)b, true, (uint8_t)d))
                        ndp[c][d] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        memcpy(dp, ndp, sizeof(dp));
    }

    /* Final check for the bottom row (no below neighbours) */
    seq_t result = 0;
    for (int l = 0; l < 16; ++l) {
        for (int r = 0; r < 16; ++r) {
            seq_t cnt = dp[l][r];
            if (cnt == 0) continue;
            if (row_valid(r, true, (uint8_t)l, false, 0))
                result += cnt;
        }
    }
    return result;
}
