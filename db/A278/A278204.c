/*
 * A278204  Number of nX4 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) (1,-1) (1,0) or (1,1), with upper left element zero.
 * https://oeis.org/A278204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×4 binary arrays with the property that each
 *            element has at least one equal neighbour and at least one
 *            different neighbour among its 8 (≤8) existing neighbours, with
 *            the upper‑left element fixed to 0. The function uses a
 *            transfer‑matrix DP over pairs of consecutive rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

typedef unsigned int pattern_t;   /* 4‑bit pattern for a row (0…15) */

/* Returns true iff the 'curr' row satisfies the neighbour condition
   given an optional 'above' row and an optional 'below' row.
   has_above/below indicate whether the corresponding neighbour rows
   exist.  The arguments are:
         above : pattern of row i‑1
         curr  : pattern of row i
         below : pattern of row i+1
   For the current row all existing neighbours among the eight offsets
   are checked. */
static bool row_satisfies(pattern_t above, pattern_t curr, pattern_t below,
                          bool has_above, bool has_below)
{
    for (int j = 0; j < 4; ++j) {
        int v = (curr >> j) & 1;
        bool has_eq = false;
        bool has_ne = false;

        /* left neighbour */
        if (j > 0) {
            int nv = (curr >> (j - 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* right neighbour */
        if (j < 3) {
            int nv = (curr >> (j + 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* above neighbour */
        if (has_above) {
            int nv = (above >> j) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* above‑left neighbour */
        if (has_above && j > 0) {
            int nv = (above >> (j - 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* above‑right neighbour */
        if (has_above && j < 3) {
            int nv = (above >> (j + 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* below neighbour */
        if (has_below) {
            int nv = (below >> j) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* below‑left neighbour */
        if (has_below && j > 0) {
            int nv = (below >> (j - 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }
        /* below‑right neighbour */
        if (has_below && j < 3) {
            int nv = (below >> (j + 1)) & 1;
            if (nv == v) has_eq = true; else has_ne = true;
        }

        if (!(has_eq && has_ne))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* DP tables: dp[prev][curr] stores the number of ways after
       processing up to the current row, where 'prev' is the previous
       row and 'curr' is the current row (whose satisfaction is still
       pending). */
    seq_t dp[16][16]   = {0};
    seq_t newdp[16][16] = {0};

    /* Initialise with the first two rows.  The top‑left element
       (bit 0 of the first row) must be 0.  */
    for (pattern_t pre = 0; pre < 16; ++pre) {
        if (pre & 1) continue;               /* bit 0 must be 0 */
        for (pattern_t cur = 0; cur < 16; ++cur) {
            /* Row 0 must satisfy using row 1 (cur) as the only below
               neighbours. */
            if (row_satisfies(0, pre, cur, false, true))
                dp[pre][cur] += 1;
        }
    }

    if (n == 2) {
        /* Validate the last row (row 1) with row 0 only. */
        seq_t total = 0;
        for (int pre = 0; pre < 16; ++pre)
            for (int cur = 0; cur < 16; ++cur)
                if (dp[pre][cur] != 0 &&
                    row_satisfies(pre, cur, 0, true, false))
                    total += dp[pre][cur];
        return total;
    }

    /* For n ≥ 3 iterate over rows 2 … n‑1. */
    for (idx_t row = 2; row < n; ++row) {
        /* Reset newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (int pre = 0; pre < 16; ++pre)
            for (int curr = 0; curr < 16; ++curr) {
                seq_t cnt = dp[pre][curr];
                if (!cnt) continue;
                for (int next = 0; next < 16; ++next)
                    if (row_satisfies(pre, curr, next, true, true))
                        newdp[curr][next] += cnt;
            }

        /* Copy newdp into dp for next iteration. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Final row (row n‑1) must satisfy with only the row above. */
    seq_t total = 0;
    for (int pre = 0; pre < 16; ++pre)
        for (int cur = 0; cur < 16; ++cur)
            if (dp[pre][cur] != 0 &&
                row_satisfies(pre, cur, 0, true, false))
                total += dp[pre][cur];

    return total;
}
