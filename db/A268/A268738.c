/*
 * A268738  Number of nX6 binary arrays with some 1 horizontally or vertically
 *          adjacent to some other 1 exactly once.
 * https://oeis.org/A268738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows for A268738. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute adjacency counts for all 6‑bit patterns. */
    static bool done = false;
    static uint8_t hcount[64];
    static uint8_t vcount[64][64];
    if (!done) {
        for (int p = 0; p < 64; ++p) {
            uint8_t hc = 0;
            for (int i = 0; i < 5; ++i)
                if (((p >> i) & 3) == 3)
                    ++hc;
            hcount[p] = hc;
            for (int q = 0; q < 64; ++q)
                vcount[p][q] = __builtin_popcount((unsigned)(p & q));
        }
        done = true;
    }

    /* DP tables: dp_cur[pattern][adj] holds counts for processed rows. */
    static seq_t dp_cur[64][2];
    static seq_t dp_next[64][2];

    /* Initialise for the first row. */
    for (int p = 0; p < 64; ++p) {
        dp_cur[p][0] = 0;
        dp_cur[p][1] = 0;
    }
    for (int p = 0; p < 64; ++p) {
        uint8_t hc = hcount[p];
        if (hc == 0)
            dp_cur[p][0] = 1;
        else if (hc == 1)
            dp_cur[p][1] = 1;
        /* patterns with hc >= 2 are discarded */
    }

    /* Process the remaining rows. */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 64; ++i) {
            dp_next[i][0] = 0;
            dp_next[i][1] = 0;
        }

        for (int p = 0; p < 64; ++p) {
            seq_t val0 = dp_cur[p][0];
            seq_t val1 = dp_cur[p][1];
            if (val0 == 0 && val1 == 0)
                continue;

            for (int q = 0; q < 64; ++q) {
                uint8_t hc_q = hcount[q];
                uint8_t v   = vcount[p][q];

                if (val0) {
                    uint8_t nk = v + hc_q;
                    if (nk <= 1)
                        dp_next[q][nk] += val0;
                }
                if (val1) {
                    uint8_t nk = v + hc_q + 1;
                    if (nk <= 1)
                        dp_next[q][nk] += val1;
                }
            }
        }

        for (int i = 0; i < 64; ++i) {
            dp_cur[i][0] = dp_next[i][0];
            dp_cur[i][1] = dp_next[i][1];
        }
    }

    /* Sum all states with exactly one adjacency. */
    seq_t ans = 0;
    for (int p = 0; p < 64; ++p)
        ans += dp_cur[p][1];

    return ans;
}
