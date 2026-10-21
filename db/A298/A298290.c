/*
 * A298290  Number of n X 4 0..1 arrays with every element equal to 0, 1, 2, 4
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays whose every element has 0, 1,
 *            2, 4, or 6 king‑move neighbors equal to its own value, with the
 *            upper‑left element forced to 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 61, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool check_row(int mid, int above, int below)
{
    /* above and below equal 16 mean no such row exists */
    for (int c = 0; c < 4; ++c) {
        int val = (mid >> c) & 1;
        int cnt = 0;
        /* same row neighbors (left/right) */
        for (int dc = -1; dc <= 1; ++dc) {
            if (dc == 0) continue;
            int cc = c + dc;
            if (cc < 0 || cc >= 4) continue;
            int nval = (mid >> cc) & 1;
            if (nval == val) ++cnt;
        }
        /* above row */
        if (above != 16) {
            for (int dc = -1; dc <= 1; ++dc) {
                int cc = c + dc;
                if (cc < 0 || cc >= 4) continue;
                int nval = (above >> cc) & 1;
                if (nval == val) ++cnt;
            }
        }
        /* below row */
        if (below != 16) {
            for (int dc = -1; dc <= 1; ++dc) {
                int cc = c + dc;
                if (cc < 0 || cc >= 4) continue;
                int nval = (below >> cc) & 1;
                if (nval == val) ++cnt;
            }
        }
        /* allowed counts 0,1,2,4,6 */
        if (cnt != 0 && cnt != 1 && cnt != 2 && cnt != 4 && cnt != 6)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset = 1; behaviour for n<1 irrelevant */

    const int MAXR = 16;             /* sentinel value: no row present */
    static bool allowed[17][16][16] = {0};
    static bool init_allowed = false;
    if (!init_allowed) {
        init_allowed = true;
        for (int pp = 0; pp <= MAXR; ++pp)
            for (int p = 0; p < 16; ++p)
                for (int c = 0; c < 16; ++c)
                    allowed[pp][p][c] = check_row(p, pp, c);
    }

    static seq_t dp[17][16];
    static seq_t ndp[17][16];

    /* initialize DP for row 1 (top row) */
    for (int pp = 0; pp <= MAXR; ++pp)
        for (int p = 0; p < 16; ++p)
            dp[pp][p] = 0;
    for (int p = 0; p < 16; ++p) {
        if ((p & 1) == 0)                 /* upper-left (bit 0) must be 0 */
            dp[MAXR][p] = 1;
    }

    /* process rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int pp = 0; pp <= MAXR; ++pp)
            for (int p = 0; p < 16; ++p)
                ndp[pp][p] = 0;

        for (int pp = 0; pp <= MAXR; ++pp) {
            for (int p = 0; p < 16; ++p) {
                seq_t cur = dp[pp][p];
                if (cur == 0) continue;
                for (int c = 0; c < 16; ++c) {
                    if (!allowed[pp][p][c]) continue;
                    ndp[p][c] += cur;
                }
            }
        }
        /* swap dp and ndp for next iteration */
        for (int pp = 0; pp <= MAXR; ++pp)
            for (int p = 0; p < 16; ++p)
                dp[pp][p] = ndp[pp][p];
    }

    /* final row must satisfy constraints with above row and no below row */
    seq_t total = 0;
    for (int pp = 0; pp <= MAXR; ++pp) {
        for (int p = 0; p < 16; ++p) {
            seq_t cur = dp[pp][p];
            if (cur == 0) continue;
            if (check_row(p, pp, MAXR))
                total += cur;
        }
    }
    return total;
}
