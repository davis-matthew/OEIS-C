/*
 * A268785  Number of nX4 binary arrays with some element plus some
 *          horizontally, vertically or antidiagonally adjacent neighbor
 *          totalling two exactly once.
 * https://oeis.org/A268785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with exactly one * adjacent
 *            pair of 1s (horizontal, vertical or antidiagonal)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

/* Pre‑computed adjacency data – computed once */
static bool __initialized = false;
static int horiz[16];
static int vertical_inc[16][16];
static int antid_inc[16][16];
static int total_inc[16][16];

static void init_tables(void)
{
    if (__initialized)
        return;
    /* horizontal adjacency within the same row */
    for (int pat = 0; pat < 16; ++pat) {
        int h = 0;
        for (int j = 0; j < 3; ++j)
            if ((pat >> j & 1) && (pat >> (j + 1) & 1))
                ++h;
        horiz[pat] = h;
    }
    /* between consecutive rows */
    for (int p = 0; p < 16; ++p) {
        for (int q = 0; q < 16; ++q) {
            int vert  = __builtin_popcount((unsigned int)(p & q));
            int antid = __builtin_popcount((unsigned int)(q & (p << 1)));
            vertical_inc[p][q] = vert;
            antid_inc[p][q]    = antid;
            total_inc[p][q]    = horiz[q] + vert + antid;
        }
    }
    __initialized = true;
}

/* Main function */
seq_t a(idx_t n)
{
    init_tables();

    /* two DP layers: current and next */
    seq_t dp[2][16][2];
    int cur = 0, nxt = 1;
    /* initialise to zeros */
    for (int pat = 0; pat < 16; ++pat)
        dp[cur][pat][0] = dp[cur][pat][1] = 0;

    /* --- set up for first row --- */
    for (int pat = 0; pat < 16; ++pat) {
        int h = horiz[pat];
        if (h <= 1)
            dp[cur][pat][h] = 1;
    }

    if (n == 1) {
        seq_t ans = 0;
        for (int pat = 0; pat < 16; ++pat)
            ans += dp[cur][pat][1];
        return ans;
    }

    /* --- process remaining rows --- */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next layer */
        for (int pat = 0; pat < 16; ++pat) {
            dp[nxt][pat][0] = 0;
            dp[nxt][pat][1] = 0;
        }

        /* transition */
        for (int prev = 0; prev < 16; ++prev) {
            for (int k = 0; k <= 1; ++k) {
                seq_t val = dp[cur][prev][k];
                if (val == 0)
                    continue;
                for (int next = 0; next < 16; ++next) {
                    int inc  = total_inc[prev][next];
                    int nk   = k + inc;
                    if (nk > 1)
                        continue;
                    dp[nxt][next][nk] += val;
                }
            }
        }
        /* swap layers */
        cur ^= 1;
        nxt ^= 1;
    }

    /* final answer: exactly one adjacency pair so far */
    seq_t ans = 0;
    for (int pat = 0; pat < 16; ++pat)
        ans += dp[cur][pat][1];
    return ans;
}
