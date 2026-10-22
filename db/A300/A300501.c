/*
 * A300501  Number of nX3 0..1 arrays with every element equal to 0, 1, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 26 * a(n) =
 *            5*a(n-2)+2*a(n-3)-7*a(n-4)-8*a(n-5)-5*a(n-6)+10*a(n-7)+34*a(n-8)+10*a(n-9)
 *            *
 *            -54*a(n-10)-52*a(n-11)+26*a(n-12)+72*a(n-13)+26*a(n-14)-40*a(n-15)
 *            *
 *            -49*a(n-16)-16*a(n-17)+21*a(n-18)+36*a(n-19)+14*a(n-20)-12*a(n-21)
 *            * -10*a(n-22)-4*a(n-23)-a(n-24)+2*a...
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper that checks whether a single row satisfies the neighbor rule,
 * given its upper and lower rows when they exist. */
static inline bool row_valid(int prev, int cur, int next, bool hasPrev, bool hasNext) {
    for (int j = 0; j < 3; ++j) {
        int val = (cur >> j) & 1;
        int same = 0;
        /* horizontal neighbors */
        if (j > 0 && ((cur >> (j - 1)) & 1) == val) ++same;
        if (j < 2 && ((cur >> (j + 1)) & 1) == val) ++same;
        /* vertical neighbors */
        if (hasPrev && ((prev >> j) & 1) == val) ++same;
        if (hasNext && ((next >> j) & 1) == val) ++same;
        /* anti‑diagonal neighbors */
        if (hasPrev && j < 2 && ((prev >> (j + 1)) & 1) == val) ++same;
        if (hasNext && j > 0 && ((next >> (j - 1)) & 1) == val) ++same;

        if (same != 0 && same != 1 && same != 3 && same != 4)
            return false;
    }
    return true;
}

/* Main function required by the OEIS build */
seq_t a(idx_t n) {
    if (n < 1) return 0;

    /* States: 0..7 represent 3‑bit rows, 8 represents “none” (outside the array). */
    const int NONE = 8;
    /* Dynamic programming tables: level[i][prev][cur] */
    seq_t dp[9][8] = {0};
    seq_t next[9][8] = {0};

    /* Initial row (row 1).  Upper‑left bit (column 0) must be 0. */
    for (int r = 0; r < 8; ++r) {
        if ((r & 1) == 0)   /* bit 0 is 0 */
            dp[NONE][r] = 1;
    }

    /* Special case: n = 1 – no neighbours below, so we evaluate directly. */
    if (n == 1) {
        seq_t result = 0;
        for (int r = 0; r < 8; ++r)
            if (dp[NONE][r] && row_valid(0, r, 0, false, false))
                result += dp[NONE][r];
        return result;
    }

    /* Iterate rows 2 … n.  dp holds counts after processing the previous row. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero next table */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = 0;

        for (int prevPrev = 0; prevPrev <= 8; ++prevPrev) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prevPrev][cur];
                if (!ways) continue;
                bool hasPrev = (prevPrev != NONE);
                for (int nextRow = 0; nextRow < 8; ++nextRow) {
                    if (!row_valid(prevPrev, cur, nextRow, hasPrev, true))
                        continue;
                    next[cur][nextRow] += ways;
                }
            }
        }
        /* Move to next level */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* After processing n rows, validate the last row (no neighbor below). */
    seq_t result = 0;
    for (int prev = 0; prev <= 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (row_valid(prev, cur, 0, true, false))
                result += ways;
        }
    }
    return result;
}
