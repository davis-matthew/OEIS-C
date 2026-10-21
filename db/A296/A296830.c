/*
 * A296830  Number of nX4 0..1 arrays with each 1 adjacent to 0, 3 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of nX4 0..1 arrays with each 1 adjacent to 0, 3 or 5 king-move neighboring 1s.
   The DP algorithm considers each row (4 bits) as a state.
   Transition: when adding a new row the middle row must be valid with the
   previous and new rows.  The first row must be valid with only the second row,
   and the last row must be valid with only the previous row.  All counts
   are performed in seq_t.  Complexity O(n * 16^3).  For n up to 10 000
   this runs easily within the limits. */
#include <stdbool.h>

static inline bool is_valid_row(int row, int above, int below,
                                bool has_above, bool has_below)
{
    for (int j = 0; j < 4; ++j) {
        if (!((row >> j) & 1))
            continue;   /* not a 1 cell */

        int cnt = 0;

        /* neighbors from the preceding row (if any) */
        if (has_above) {
            if (j > 0 && ((above >> (j - 1)) & 1)) ++cnt;
            if ((above >> j) & 1)                  ++cnt;
            if (j < 3 && ((above >> (j + 1)) & 1)) ++cnt;
        }

        /* horizontal neighbors in the same row */
        if (j > 0 && ((row >> (j - 1)) & 1)) ++cnt;
        if (j < 3 && ((row >> (j + 1)) & 1)) ++cnt;

        /* neighbors from the following row (if any) */
        if (has_below) {
            if (j > 0 && ((below >> (j - 1)) & 1)) ++cnt;
            if ((below >> j) & 1)                  ++cnt;
            if (j < 3 && ((below >> (j + 1)) & 1)) ++cnt;
        }

        /* each 1 must have 0, 3 or 5 neighboring 1s */
        if (cnt != 0 && cnt != 3 && cnt != 5)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, but keeps the function safe */

    /* base n = 1 */
    if (n == 1) {
        seq_t cnt = 0;
        for (int mask = 0; mask < 16; ++mask) {
            if (is_valid_row(mask, 0, 0, false, false))
                ++cnt;
        }
        return cnt;   /* equals 8 */
    }

    /* DP arrays: dp[prev][curr] = number of ways to reach state after curr */
    seq_t dp[16][16] = {0};
    seq_t ndp[16][16] = {0};

    /* initialise with all pairs (row1,row2) where row1 is valid with only below row2 */
    for (int r1 = 0; r1 < 16; ++r1)
        for (int r2 = 0; r2 < 16; ++r2)
            if (is_valid_row(r1, 0, r2, false, true))
                dp[r1][r2] = 1;

    /* add rows 3 .. n */
    for (idx_t i = 3; i <= n; ++i) {
        /* clear ndp */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                ndp[p][c] = 0;

        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (is_valid_row(curr, prev, nxt, true, true))
                        ndp[curr][nxt] += ways;
                }
            }

        /* move to next step */
        for (int p = 0; p < 16; ++p)
            for (int c = 0; c < 16; ++c)
                dp[p][c] = ndp[p][c];
    }

    /* final row validation: last row must be valid with only above row */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr) {
            seq_t ways = dp[prev][curr];
            if (!ways) continue;
            if (is_valid_row(curr, prev, 0, true, false))
                ans += ways;
        }

    return ans;
}
