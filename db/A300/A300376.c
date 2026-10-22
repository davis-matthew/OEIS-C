/*
 * A300376  Number of nX4 0..1 arrays with every element equal to 0, 2 or 3
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×4 binary arrays with each element *
 *            having 0, 2 or 3 equal neighbours among the horizontal, vertical
 *            and * anti‑diagonal directions; the upper‑left corner is fixed
 *            to 0. * This is computed by a row‑by‑row DP that keeps the last
 *            two rows * as state. The DP uses 6 neighbour...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   valid_row_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/valid_row_mask.h"

/* pre‑computed tables that avoid recomputing the neighbour checks
 * during the DP.  The tables are built lazily on the first call. */
static bool precomp_done = false;
static bool valid_mid[2][16][16][16];
static bool valid_last[16][16];

static void precompute_tables(void)
{
    for (int top = 0; top <= 1; ++top) {
        for (int prev = 0; prev < 16; ++prev)
            for (int cur = 0; cur < 16; ++cur)
                for (int next = 0; next < 16; ++next)
                    valid_mid[top][prev][cur][next] =
                        valid_row_mask(prev, cur, next, top != 0, false);
    }
    for (int prev = 0; prev < 16; ++prev)
        for (int cur = 0; cur < 16; ++cur)
            valid_last[prev][cur] =
                valid_row_mask(prev, cur, 0, false, true);
    precomp_done = true;
}

/* DP arrays used for the computation.  They are static so that the
 * function is re‑entrant; their values are overwritten for each call. */
static seq_t dp[16][16];
static seq_t nextdp[16][16];

seq_t a(idx_t n)
{
    /* any calls with n < 1 return 0; the defined offset is 1. */
    if (n < 1) return 0;

    if (!precomp_done)
        precompute_tables();

    /* initialise dp with only the first row known.  The fictitious
     * row above row 0 is represented by mask 0, but neighbours above
     * are ignored because 'top' is true when validating row 0. */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    for (int r0 = 0; r0 < 16; ++r0)
        if ((r0 & 1) == 0)        /* upper‑left element must be 0 */
            dp[0][r0] = 1;

    /* handle the trivial case n == 1 separately.  We must validate
     * the single row with both top and bottom ignored. */
    if (n == 1) {
        seq_t ans = 0;
        for (int r0 = 0; r0 < 16; ++r0) {
            if ((r0 & 1) != 0) continue;
            if (valid_row_mask(0, r0, 0, true, true))
                ans += dp[0][r0];
        }
        return ans;
    }

    /* add the remaining rows one by one.  For the transition that
     * validates row 0 ('top' flag is true) the loop index is 1.  For
     * all later transitions 'top' is false. */
    for (idx_t row_index = 1; row_index < n; ++row_index) {
        /* clear nextdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                nextdp[i][j] = 0;

        bool top_flag = (row_index == 1);   /* only the first transition */
        for (int prev = 0; prev < 16; ++prev) {
            for (int cur = 0; cur < 16; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (!cnt) continue;
                for (int next = 0; next < 16; ++next) {
                    if (!valid_mid[top_flag][prev][cur][next])
                        continue;
                    nextdp[cur][next] += cnt;
                }
            }
        }

        /* copy nextdp back into dp for the next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* after all rows are processed, dp holds the counts for the last
     * two rows (prev = n-2, cur = n-1).  Validate the last row with
     * 'bottom' set to true. */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            seq_t cnt = dp[prev][cur];
            if (!cnt) continue;
            if (valid_last[prev][cur])
                ans += cnt;
        }
    }
    return ans;
}
