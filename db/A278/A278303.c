/*
 * A278303  Number of n X 2 0..2 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly one mistake.
 * https://oeis.org/A278303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays with entries 0..2 whose rows *
 *            (considered as tuples) and columns (considered as vectors) are
 *            in * lexicographic nondecreasing order, but with exactly one
 *            violation of * these monotonicity rules. This implementation
 *            counts all arrays * with either exactly one row‑wise...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 88, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Mapping from row index (0..8) to its two entries (x,y).  The rows
 * are taken in lexicographic order over {0,1,2}×{0,1,2}. */
static const uint8_t row_x[9] = {0,0,0,1,1,1,2,2,2};
static const uint8_t row_y[9] = {0,1,2,0,1,2,0,1,2};

/* DP arrays: dp[cmp][vt][row] where
 *   cmp 0 = columns identical so far (x==y for all processed rows)
 *        1 = columns already determined to be lexicographically smaller
 *        2 = columns already determined to be lexicographically larger
 *   vt   0 = no row violation encountered so far
 *        1 = exactly one row violation has occurred
 *   row  index 0..8 of the last processed row.
 * Values are stored in seq_t. */
static seq_t dp[3][2][9];
static seq_t ndp[3][2][9];

/* Utility to zero a DP array. */
static void zero_dp(void) {
    for (idx_t cmp = 0; cmp < 3; ++cmp)
        for (idx_t vt = 0; vt < 2; ++vt)
            for (idx_t r = 0; r < 9; ++r)
                dp[cmp][vt][r] = 0;
}

static void zero_ndp(void) {
    for (idx_t cmp = 0; cmp < 3; ++cmp)
        for (idx_t vt = 0; vt < 2; ++vt)
            for (idx_t r = 0; r < 9; ++r)
                ndp[cmp][vt][r] = 0;
}

/* Main counting function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* Undefined before offset, return 0 */
    /* -------------------------------------------------------------
     * Initialization for first row (i = 1).
     * ------------------------------------------------------------- */
    zero_dp();
    for (idx_t v = 0; v < 9; ++v) {
        uint8_t x = row_x[v];
        uint8_t y = row_y[v];
        idx_t cmp;
        if (x < y) cmp = 1;
        else if (x > y) cmp = 2;
        else cmp = 0;
        dp[cmp][0][v] = 1;           /* vt = 0, single row, no violation */
    }

    /* -------------------------------------------------------------
     * DP for rows 2..n.
     * ------------------------------------------------------------- */
    for (idx_t i = 2; i <= n; ++i) {
        zero_ndp();
        for (idx_t cmp0 = 0; cmp0 < 3; ++cmp0) {
            for (idx_t vt0 = 0; vt0 < 2; ++vt0) {
                for (idx_t r0 = 0; r0 < 9; ++r0) {
                    seq_t cur = dp[cmp0][vt0][r0];
                    if (cur == 0) continue;
                    for (idx_t r1 = 0; r1 < 9; ++r1) {
                        /* Determine new vt (row violations). */
                        idx_t vt1 = vt0;
                        if (vt0 == 0) {
                            if (r1 < r0) vt1 = 1;   /* new violation */
                            /* otherwise vt1 stays 0 */
                        } else {
                            if (r1 < r0) continue;  /* second violation not allowed */
                            /* vt1 stays 1 */
                        }

                        /* Determine new cmp (column comparison). */
                        idx_t cmp1 = cmp0;
                        if (cmp0 == 0) {
                            uint8_t x = row_x[r1];
                            uint8_t y = row_y[r1];
                            if (x < y) cmp1 = 1;
                            else if (x > y) cmp1 = 2;
                            else cmp1 = 0;
                        }   /* cmp stays 1 or 2 otherwise */

                        ndp[cmp1][vt1][r1] += cur;
                    }
                }
            }
        }
        /* swap dp and ndp */
        for (idx_t cmp = 0; cmp < 3; ++cmp)
            for (idx_t vt = 0; vt < 2; ++vt)
                for (idx_t r = 0; r < 9; ++r)
                    dp[cmp][vt][r] = ndp[cmp][vt][r];
    }

    /* -------------------------------------------------------------
     * Aggregate counts:
     *   f(n) = dp[0|1][0][*]   (good rows, good columns)
     *   h(n) = dp[2][0][*]     (good rows, column violation)
     *   g(n) = dp[0|1][1][*]   (exactly one row violation, good columns)
     * ------------------------------------------------------------- */
    seq_t sum_f = 0, sum_h = 0, sum_g = 0;
    for (idx_t r = 0; r < 9; ++r) {
        sum_f += dp[0][0][r];
        sum_f += dp[1][0][r];
        sum_h += dp[2][0][r];
        sum_g += dp[0][1][r];
        sum_g += dp[1][1][r];
    }
    return sum_g + sum_h;   /* a(n) = g(n) + h(n) */
}
