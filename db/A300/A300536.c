/*
 * A300536  Number of nX5 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, vertically or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A300536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays with the upper‑left element 0
 *            and for every element the number of horizontally, vertically or
 *            antidiagonally adjacent equal elements is 0, 2, 3 or 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>


static inline bool check_row(uint8_t r, bool haveUp, uint8_t up,
                             bool haveDown, uint8_t down)
{
    for (int c = 0; c < 5; ++c) {
        int cell = (r >> c) & 1;
        int same = 0;

        /* left neighbour */
        if (c > 0) {
            int nb = (r >> (c - 1)) & 1;
            same += (nb == cell);
        }
        /* right neighbour */
        if (c < 4) {
            int nb = (r >> (c + 1)) & 1;
            same += (nb == cell);
        }
        /* upper neighbours (row above) */
        if (haveUp) {
            int nb = (up >> c) & 1;
            same += (nb == cell);
            /* up‑right neighbour (antidiagonal) */
            if (c < 4) {
                nb = (up >> (c + 1)) & 1;
                same += (nb == cell);
            }
        }
        /* lower neighbours (row below) */
        if (haveDown) {
            int nb = (down >> c) & 1;
            same += (nb == cell);
            /* down‑left neighbour (antidiagonal) */
            if (c > 0) {
                nb = (down >> (c - 1)) & 1;
                same += (nb == cell);
            }
        }

        if (same != 0 && same != 2 && same != 3 && same != 4)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* Pre‑compute transitions: trans[prevPrev][prev] = bitmask of
       rows that can follow.  Computed only once. */
    static bool init = false;
    static uint32_t trans[32][32];
    if (!init) {
        init = true;
        for (uint8_t pp = 0; pp < 32; ++pp) {
            for (uint8_t p = 0; p < 32; ++p) {
                uint32_t bm = 0;
                for (uint8_t d = 0; d < 32; ++d) {
                    if (check_row(p, true, pp, true, d))
                        bm |= 1u << d;
                }
                trans[pp][p] = bm;
            }
        }
    }

    /* Special case: n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t m = 0; m < 32; ++m) {
            if (m & 1) continue;          /* upper‑left must be 0 */
            if (check_row(m, false, 0, false, 0))
                ans += 1;
        }
        return ans;
    }

    static seq_t dp[32][32], dp2[32][32];

    /* initialise dp for the first two rows */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    for (uint8_t m0 = 0; m0 < 32; ++m0) {
        if (m0 & 1) continue;              /* upper‑left must be 0 */
        for (uint8_t m1 = 0; m1 < 32; ++m1) {
            if (check_row(m0, false, 0, true, m1))
                dp[m0][m1] = 1;
        }
    }

    /* Process rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp2[i][j] = 0;

        for (int pp = 0; pp < 32; ++pp) {
            for (int p = 0; p < 32; ++p) {
                seq_t val = dp[pp][p];
                if (val == 0) continue;
                uint32_t bm = trans[pp][p];
                while (bm) {
                    uint32_t d = __builtin_ctz(bm);
                    bm &= bm - 1;
                    dp2[p][d] += val;
                }
            }
        }

        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = dp2[i][j];
    }

    /* Final row constraints */
    seq_t ans = 0;
    for (int pp = 0; pp < 32; ++pp) {
        for (int p = 0; p < 32; ++p) {
            seq_t val = dp[pp][p];
            if (val == 0) continue;
            if (check_row(p, true, pp, false, 0))
                ans += val;
        }
    }
    return ans;
}
