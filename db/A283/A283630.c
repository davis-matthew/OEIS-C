/*
 * A283630  Number of nX4 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A283630
   Computation by dynamic programming over rows with a state of two previous
   rows (4‑bit patterns) and the number of violating cells already found
   (0..2).  After all rows are processed a dummy zero row is appended to
   finalize the last row.  The transition counts how many cells in the
   previous row exceed the allowed number of 1‑neighbors; the
   transition is allowed only if the total number of violations does not
   exceed 2. */

#include <stddef.h>
#include <stdbool.h>

/* The function prototype is required; seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Pre‑compute the number of violations for every triple of
       (previous–previous row, previous row, current row).  The grid has
       width 4, so each row is a 4‑bit pattern (0..15).  The table is
       built only once. */
    static unsigned char viol[16][16][16];
    static bool init = false;
    if (!init) {
        for (int pp = 0; pp < 16; ++pp) {
            for (int p = 0; p < 16; ++p) {
                for (int c = 0; c < 16; ++c) {
                    unsigned v = 0;
                    for (int j = 0; j < 4; ++j) {
                        if (!(p & (1 << j))) continue;
                        unsigned cnt = 0;
                        /* horizontal neighbors */
                        if (j > 0 && (p & (1 << (j - 1)))) ++cnt;
                        if (j < 3 && (p & (1 << (j + 1)))) ++cnt;
                        /* diagonal neighbors from previous previous row */
                        if (j > 0 && (pp & (1 << (j - 1)))) ++cnt;
                        if (j < 3 && (pp & (1 << (j + 1)))) ++cnt;
                        /* diagonal neighbors from current row */
                        if (j > 0 && (c & (1 << (j - 1)))) ++cnt;
                        if (j < 3 && (c & (1 << (j + 1)))) ++cnt;
                        if (cnt > 1) ++v;
                    }
                    viol[pp][p][c] = (unsigned char)v;
                }
            }
        }
        init = true;
    }

    /* DP tables: dp[prevPrev][prev][e]   e = 0..2 number of violations seen */
    static seq_t dp[16][16][3];
    static seq_t ndp[16][16][3];

    /* initialise dp */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;
    dp[0][0][0] = 1;                  /* start with two empty rows */

    /* process each of the n real rows */
    for (idx_t row = 0; row < n; ++row) {
        /* clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 3; ++k)
                    ndp[i][j][k] = 0;

        for (int pp = 0; pp < 16; ++pp) {
            for (int p = 0; p < 16; ++p) {
                for (int e = 0; e <= 2; ++e) {
                    seq_t val = dp[pp][p][e];
                    if (val == 0) continue;
                    for (int c = 0; c < 16; ++c) {
                        unsigned v = viol[pp][p][c];
                        if (e + v > 2) continue;
                        ndp[p][c][e + v] += val;
                    }
                }
            }
        }

        /* move ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* append a dummy zero row to finalize the last real row */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            for (int k = 0; k < 3; ++k)
                ndp[i][j][k] = 0;

    for (int pp = 0; pp < 16; ++pp) {
        for (int p = 0; p < 16; ++p) {
            for (int e = 0; e <= 2; ++e) {
                seq_t val = dp[pp][p][e];
                if (val == 0) continue;
                unsigned v = viol[pp][p][0];  /* cur = 0 row */
                if (e + v > 2) continue;
                ndp[p][0][e + v] += val;
            }
        }
    }

    /* sum all states that have exactly two violations */
    seq_t result = 0;
    for (int pp = 0; pp < 16; ++pp)
        for (int p = 0; p < 16; ++p)
            result += ndp[pp][p][2];

    return result;
}
