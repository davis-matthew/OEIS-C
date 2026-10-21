/*
 * A299577  Number of nX4 0..1 arrays with every element equal to 0, 3, 4, 5,
 *          7 or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0/1 arrays with each element equal to 0, 3,
 *            4, 5, 7 or 8 king-move adjacent elements, with upper left
 *            element zero. The algorithm uses dynamic programming over rows
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


#define NO_ROW 16  /* sentinel for a missing row */

static bool row_valid(unsigned row, bool above_exists, unsigned above,
                      bool below_exists, unsigned below)
{
    for (int c = 0; c < 4; ++c) {
        unsigned val = (row >> c) & 1;
        int cnt = 0;

        /* same row neighbors */
        if (c > 0)      { unsigned nb = (row >> (c - 1)) & 1; if (nb == val) ++cnt; }
        if (c < 3)      { unsigned nb = (row >> (c + 1)) & 1; if (nb == val) ++cnt; }

        /* above row neighbors */
        if (above_exists) {
            if (c > 0)      { unsigned nb = (above >> (c - 1)) & 1; if (nb == val) ++cnt; }
            unsigned nb2 = (above >> c) & 1; if (nb2 == val) ++cnt;
            if (c < 3)      { unsigned nb = (above >> (c + 1)) & 1; if (nb == val) ++cnt; }
        }

        /* below row neighbors */
        if (below_exists) {
            if (c > 0)      { unsigned nb = (below >> (c - 1)) & 1; if (nb == val) ++cnt; }
            unsigned nb2 = (below >> c) & 1; if (nb2 == val) ++cnt;
            if (c < 3)      { unsigned nb = (below >> (c + 1)) & 1; if (nb == val) ++cnt; }
        }

        /* allowed counts are 0,3,4,5,7,8 */
        if (!(cnt == 0 || (cnt >= 3 && cnt != 6)))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 1, but harmless */

    /* DP arrays: dp[prevprev][prev] */
    seq_t dp[NO_ROW + 1][16] = {0};
    seq_t ndp[NO_ROW + 1][16] = {0};

    /* Initial state: first row, no previous row (prevprev = NO_ROW) */
    for (unsigned r = 0; r < 16; ++r) {
        if ((r & 1) == 0)          /* upper left element zero */
            dp[NO_ROW][r] = (seq_t)1;
    }

    /* Process remaining rows */
    for (idx_t row = 1; row < n; ++row) {
        /* zero ndp */
        for (int sp = 0; sp <= NO_ROW; ++sp)
            for (int s = 0; s < 16; ++s)
                ndp[sp][s] = (seq_t)0;

        for (int sp = 0; sp <= NO_ROW; ++sp) {
            bool above_exists = (sp != NO_ROW);
            unsigned above = above_exists ? (unsigned)sp : 0;
            for (int s = 0; s < 16; ++s) {
                seq_t cur = dp[sp][s];
                if (!cur) continue;

                for (unsigned curr = 0; curr < 16; ++curr) {
                    if (!row_valid((unsigned)s, above_exists, above,
                                   true /* below exists */, curr))
                        continue;
                    ndp[s][curr] += cur;
                }
            }
        }

        /* copy ndp to dp for next iteration */
        for (int sp = 0; sp <= NO_ROW; ++sp)
            for (int s = 0; s < 16; ++s)
                dp[sp][s] = ndp[sp][s];
    }

    /* Final validation for the last row (no below row) */
    seq_t ans = (seq_t)0;
    for (int sp = 0; sp <= NO_ROW; ++sp) {
        bool above_exists = (sp != NO_ROW);
        unsigned above = above_exists ? (unsigned)sp : 0;
        for (int s = 0; s < 16; ++s) {
            seq_t cur = dp[sp][s];
            if (!cur) continue;
            if (row_valid((unsigned)s, above_exists, above,
                           false /* below missing */, 0))
                ans += cur;
        }
    }

    return ans;
}
