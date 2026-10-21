/*
 * A295272  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 4 1s.
 * https://oeis.org/A295272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays in which every 1 has 0,1, or
 *            4 horizontally/vertically adjacent 1s
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   cell_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/cell_ok.h"

static inline int row_ok(int row, int up, int down)
{
    for (int pos = 0; pos < 5; ++pos)
        if (row & (1 << pos))
            if (!cell_ok(row, up, down, pos))
                return 0;
    return 1;
}

seq_t a(idx_t n)
{
    /* 1‑row case */
    if (n == (idx_t)1) {
        seq_t cnt = 0;
        for (int r = 0; r < 32; ++r)
            if (row_ok(r, 0, 0))
                ++cnt;
        return cnt;
    }

    /* Pre‑compute transition masks: mask[up][row] has bit 'next' set
       iff row_ok(row, up, next) holds.  */
    static uint32_t mask[32][32];
    static int init_mask = 0;
    if (!init_mask) {
        for (int up = 0; up < 32; ++up) {
            for (int row = 0; row < 32; ++row) {
                uint32_t m = 0;
                for (int next = 0; next < 32; ++next)
                    if (row_ok(row, up, next))
                        m |= 1u << next;
                mask[up][row] = m;
            }
        }
        init_mask = 1;
    }

    static seq_t dp[32][32];
    static seq_t newdp[32][32];
    memset(dp, 0, sizeof(dp));

    /* Initialise for the first two rows (first row has no up neighbour). */
    for (int first = 0; first < 32; ++first) {
        uint32_t m = mask[0][first];
        for (int s = 0; s < 32; ++s)
            if (m & (1u << s))
                ++dp[first][s];
    }

    /* Extend rows from 3 to n */
    for (idx_t step = 2; step < n; ++step) {
        memset(newdp, 0, sizeof(newdp));
        for (int up = 0; up < 32; ++up)
            for (int cur = 0; cur < 32; ++cur) {
                seq_t val = dp[up][cur];
                if (!val) continue;
                uint32_t m = mask[up][cur];
                for (int nxt = 0; nxt < 32; ++nxt)
                    if (m & (1u << nxt))
                        newdp[cur][nxt] += val;
            }
        /* Copy newdp to dp for next iteration */
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Sum over all last two rows, checking that the final row has no
       down neighbour. */
    seq_t ans = 0;
    for (int up = 0; up < 32; ++up)
        for (int cur = 0; cur < 32; ++cur)
            if (row_ok(cur, up, 0))
                ans += dp[up][cur];

    return ans;
}
