/*
 * A268624  Number of n X 4 0..2 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A268624 by dynamic programming over 81
 * 3⁴‑patterns.  The function returns the exact value when it fits
 * in the chosen seq_t type. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Number of possible rows = 3⁴ = 81 */
    enum { PAT = 81 };

    /* Helper tables, initialised once */
    static uint8_t digits[PAT][4];
    static int hcount[PAT];            /* horizontal pairs in a row   */
    static int vcount[PAT][PAT];      /* vertical pairs between rows */
    static int init = 0;

    if (!init) {
        /* Pre‑compute the digits for every pattern */
        for (int p = 0; p < PAT; ++p) {
            int t = p;
            for (int k = 0; k < 4; ++k) {
                digits[p][k] = t % 3;
                t /= 3;
            }
            /* Count horizontal pairs that sum to 2 */
            int h = 0;
            for (int k = 0; k < 3; ++k)
                if (digits[p][k] + digits[p][k + 1] == 2) ++h;
            hcount[p] = h;
        }
        /* Count vertical pairs between every pair of patterns */
        for (int p = 0; p < PAT; ++p) {
            for (int q = 0; q < PAT; ++q) {
                int v = 0;
                for (int k = 0; k < 4; ++k)
                    if (digits[p][k] + digits[q][k] == 2) ++v;
                vcount[p][q] = v;
            }
        }
        init = 1;
    }

    /* DP tables: dp0[p] – ways ending in pattern p with 0 pairs,
     *            dp1[p] – ways ending in pattern p with 1 pair.
     *          new0/new1 are temporary tables for the next row. */
    static seq_t dp0[PAT], dp1[PAT], new0[PAT], new1[PAT];

    /* Initialise for the first row */
    for (int i = 0; i < PAT; ++i) {
        dp0[i] = 0;
        dp1[i] = 0;
    }
    for (int p = 0; p < PAT; ++p) {
        if (hcount[p] == 0)
            dp0[p] = 1;
        else if (hcount[p] == 1)
            dp1[p] = 1;
    }

    /* Process rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        for (int i = 0; i < PAT; ++i) {
            new0[i] = 0;
            new1[i] = 0;
        }
        for (int prev = 0; prev < PAT; ++prev) {
            seq_t w0 = dp0[prev];
            seq_t w1 = dp1[prev];
            if (!w0 && !w1) continue;
            for (int curr = 0; curr < PAT; ++curr) {
                int add = hcount[curr] + vcount[prev][curr];
                if (w0) {
                    if (add == 0) new0[curr] += w0;
                    else if (add == 1) new1[curr] += w0;
                }
                if (w1 && add == 0) new1[curr] += w1;    /* w1 + 1 would be 2 */
            }
        }
        for (int i = 0; i < PAT; ++i) {
            dp0[i] = new0[i];
            dp1[i] = new1[i];
        }
    }

    /* Sum all valid final states */
    seq_t result = 0;
    for (int i = 0; i < PAT; ++i) {
        result += dp0[i];
        result += dp1[i];
    }
    return result;
}
