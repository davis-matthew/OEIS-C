/*
 * A298837  Number of nX4 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which each cell has a
 *            number of same‑valued king‑move neighbours in {2,3,4,5,8}; the
 *            upper‑left cell must be 0. Uses DP over rows, O(n·16³)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static const bool ALLOWED[9] = {false, false, true, true, true, true, false, false, true};

static inline bool row_ok(uint8_t above, uint8_t row, uint8_t below,
                          bool top, bool bottom)
{
    for (int col = 0; col < 4; ++col) {
        int bit = (row >> col) & 1;
        int same = 0;

        /* horizontal neighbours */
        if (col > 0 && ((row >> (col - 1)) & 1) == bit) same++;
        if (col < 3 && ((row >> (col + 1)) & 1) == bit) same++;

        /* vertical neighbours */
        if (!top && ((above >> col) & 1) == bit) same++;
        if (!bottom && ((below >> col) & 1) == bit) same++;

        /* diagonal neighbours */
        if (!top && col > 0 && ((above >> (col - 1)) & 1) == bit) same++;
        if (!top && col < 3 && ((above >> (col + 1)) & 1) == bit) same++;
        if (!bottom && col > 0 && ((below >> (col - 1)) & 1) == bit) same++;
        if (!bottom && col < 3 && ((below >> (col + 1)) & 1) == bit) same++;

        if (!ALLOWED[same]) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Pre‑compute boundary tables once */
    static bool top_valid[16][16];
    static bool mid_valid[16][16][16];
    static bool bottom_valid[16][16];
    static bool single_valid[16];
    static bool ready = false;
    if (!ready) {
        for (int prev = 0; prev < 16; ++prev) {
            if (prev & 1) continue; /* upper left must be 0 */
            for (int curr = 0; curr < 16; ++curr) {
                top_valid[prev][curr] = row_ok(0, prev, curr, true, false);
            }
        }
        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                for (int next = 0; next < 16; ++next)
                    mid_valid[prev][curr][next] =
                        row_ok(prev, curr, next, false, false);
        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                bottom_valid[prev][curr] =
                    row_ok(prev, curr, 0, false, true);
        for (int r = 0; r < 16; ++r)
            single_valid[r] = row_ok(0, r, 0, true, true);
        ready = true;
    }

    /* Special case n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int r = 0; r < 16; ++r)
            if ((r & 1) == 0 && single_valid[r])
                ans += (seq_t)1;
        return ans;
    }

    /* DP arrays: dp[prev][curr] holds number of ways where
       'prev' is row i-1 (verified) and 'curr' is row i (pending).  */
    static seq_t dp[16][16];
    static seq_t next[16][16];
    memset(dp, 0, sizeof(dp));

    /* Initialize with first two rows */
    for (int r0 = 0; r0 < 16; ++r0) {
        if (r0 & 1) continue;               /* upper left must be 0 */
        for (int r1 = 0; r1 < 16; ++r1) {
            if (top_valid[r0][r1])
                dp[r0][r1] += (seq_t)1;
        }
    }

    /* Process remaining rows if any */
    for (idx_t row = 2; row < n; ++row) { /* row indexes start at 2, the third row */
        memset(next, 0, sizeof(next));
        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t val = dp[prev][curr];
                if (!val) continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (mid_valid[prev][curr][nxt])
                        next[curr][nxt] += val;
                }
            }
        }
        memcpy(dp, next, sizeof(dp));
    }

    /* Final boundary: last row must be valid with above=prev, below none */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (dp[prev][curr] && bottom_valid[prev][curr])
                ans += dp[prev][curr];
        }
    }
    return ans;
}
