/*
 * A300086  Number of nX5 0..1 arrays with every element equal to 0, 1, 3, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over the rows of the
   n×5 binary array.  For each triple of consecutive rows (prevPrev,
   prev, curr) it checks that every element of the middle row satisfies
   the king‑move adjacency rule (the number of equal neighbors is not
   2 or 4).  Boundary rows use a special value 32 to encode the
   absence of an adjacent row.  The first row is required to have its
   upper‑left element equal to 0.  The DP computes the number of ways
   to fill all n rows and then sums the valid endings.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Precomputed table of whether a triple of rows is compatible.
   Index 32 represents “no row” (top or bottom boundary). */
static unsigned char ok[33][32][33];
static int ok_initialized = 0;

/* Test whether the middle row satisfies the constraint with the
   rows above and below (if they exist).  pp, p, c are indices:
   0..31 for real rows, 32 indicates absence. */
static inline int row_ok(int pp, int p, int c)
{
    for (int col = 0; col < 5; ++col) {
        int cell = (p >> col) & 1;
        int cnt = 0;

        /* left and right neighbors in the same row */
        if (col > 0 && ((p >> (col - 1)) & 1) == cell) ++cnt;
        if (col < 4 && ((p >> (col + 1)) & 1) == cell) ++cnt;

        /* above neighbors */
        if (pp != 32 && ((pp >> col) & 1) == cell) ++cnt;

        /* below neighbors */
        if (c != 32  && ((c  >> col) & 1) == cell) ++cnt;

        /* diagonal neighbors */
        if (pp != 32) {
            if (col > 0 && ((pp >> (col - 1)) & 1) == cell) ++cnt;
            if (col < 4 && ((pp >> (col + 1)) & 1) == cell) ++cnt;
        }
        if (c != 32) {
            if (col > 0 && ((c  >> (col - 1)) & 1) == cell) ++cnt;
            if (col < 4 && ((c  >> (col + 1)) & 1) == cell) ++cnt;
        }

        if (cnt == 2 || cnt == 4) return 0;   /* forbidden pattern */
    }
    return 1;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Lazily initialise the compatibility table. */
    if (!ok_initialized) {
        for (int pp = 0; pp <= 32; ++pp)
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c <= 32; ++c)
                    ok[pp][p][c] = row_ok(pp, p, c);
        ok_initialized = 1;
    }

    /* DP tables: dp[prevPrev][prev]; counts of ways to reach this
       pair of rows.  The third row is currently unknown. */
    static seq_t dp[33][32];
    static seq_t nextdp[33][32];

    /* initialise dp with the first real row (row 1). */
    for (int pp = 0; pp <= 32; ++pp)
        for (int p = 0; p < 32; ++p)
            dp[pp][p] = 0;
    for (int row = 0; row < 32; ++row)
        if ((row & 1) == 0)               /* upper-left must be 0 */
            dp[32][row] = 1;              /* prevPrev = 32 means “no row” */

    /* iterate over the remaining rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        /* reset nextdp */
        for (int pp = 0; pp <= 32; ++pp)
            for (int p = 0; p < 32; ++p)
                nextdp[pp][p] = 0;

        /* transition */
        for (int pp = 0; pp <= 32; ++pp) {
            for (int p = 0; p < 32; ++p) {
                seq_t val = dp[pp][p];
                if (!val) continue;
                for (int c = 0; c < 32; ++c) {
                    if (ok[pp][p][c])
                        nextdp[p][c] += val;
                }
            }
        }

        /* copy nextdp into dp for next iteration */
        for (int pp = 0; pp <= 32; ++pp)
            for (int p = 0; p < 32; ++p)
                dp[pp][p] = nextdp[pp][p];
    }

    /* finalisation: check constraints for the last row (bottom boundary).
       Below that row is absent, encoded as 32. */
    seq_t result = 0;
    for (int pp = 0; pp <= 32; ++pp) {
        for (int p = 0; p < 32; ++p) {
            if (ok[pp][p][32])
                result += dp[pp][p];
        }
    }
    return result;
}
