/*
 * A278172  Number of nX3 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) (1,-1)
 *          or (1,0), with upper left element zero.
 * https://oeis.org/A278172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via transfer‑matrix dynamic programming over rows, checking the
   0/1‑grid neighbour constraints described in A278172. */

#include <stdint.h>
#include <stdbool.h>

/* Returns true if all three cells in the current row satisfy the
   neighbour condition, given the patterns of the previous and next
   rows. If a neighbouring row is missing, hasPrev or hasNext must be
   false and its pattern is ignored. */
static bool row_ok(uint8_t prev, uint8_t cur, uint8_t next,
                   bool hasPrev, bool hasNext)
{
    for (int col = 0; col < 3; ++col) {
        uint8_t val = (cur >> col) & 1;
        bool have_equal = false;
        bool have_diff  = false;

        /* (-1,0) – up */
        if (hasPrev) {
            uint8_t v = (prev >> col) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }
        /* (-1,1) – up‑right */
        if (hasPrev && col + 1 < 3) {
            uint8_t v = (prev >> (col + 1)) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }
        /* (0,-1) – left */
        if (col - 1 >= 0) {
            uint8_t v = (cur >> (col - 1)) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }
        /* (0,1) – right */
        if (col + 1 < 3) {
            uint8_t v = (cur >> (col + 1)) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }
        /* (1,-1) – down‑left */
        if (hasNext && col - 1 >= 0) {
            uint8_t v = (next >> (col - 1)) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }
        /* (1,0) – down */
        if (hasNext) {
            uint8_t v = (next >> col) & 1;
            if (v == val) have_equal = true; else have_diff = true;
        }

        /* Each cell must have at least one neighbour equal and at least one
           neighbour different from its value. If there are fewer than two
           neighbours, this check will fail automatically. */
        if (!(have_equal && have_diff))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0U)          /* not used; a(1) is first defined term */
        return (seq_t)0;

    if (n == 1U) {
        seq_t cnt = (seq_t)0;
        for (uint8_t p0 = 0; p0 < 8; ++p0) {
            if ((p0 & 1U) == 0) {               /* upper‑left element zero */
                if (row_ok(0, p0, 0, false, false))
                    ++cnt;
            }
        }
        return cnt;
    }

    /* DP arrays: dp[prev][cur] = number of ways to reach this state. */
    seq_t dp[8][8]  = {{0}};
    seq_t ndp[8][8] = {{0}};

    /* Initial state: first two rows. */
    for (uint8_t p0 = 0; p0 < 8; ++p0) {
        if ((p0 & 1U) == 0) {               /* upper‑left element zero */
            for (uint8_t p1 = 0; p1 < 8; ++p1) {
                if (row_ok(0, p0, p1, false, true))
                    ++dp[p0][p1];
            }
        }
    }

    /* Process remaining rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row) {
        /* Reset ndp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                ndp[i][j] = (seq_t)0;

        for (uint8_t prev = 0; prev < 8; ++prev) {
            for (uint8_t cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                for (uint8_t next = 0; next < 8; ++next) {
                    if (row_ok(prev, cur, next, true, true))
                        ndp[cur][next] += ways;
                }
            }
        }

        /* Copy ndp into dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Add contributions from the last row (no next row) */
    seq_t result = (seq_t)0;
    for (uint8_t prev = 0; prev < 8; ++prev) {
        for (uint8_t cur = 0; cur < 8; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (row_ok(prev, cur, 0, true, false))
                result += ways;
        }
    }
    return result;
}
