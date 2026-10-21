/*
 * A297593  Number of nX6 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 5 neighboring 1s.
 * https://oeis.org/A297593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP counting of n×6 binary arrays where every 1 has exactly 1 or
 *            5 adjacent * 1‑cells among its horizontal and diagonal
 *            neighbours (no vertical neighbours). * The algorithm uses
 *            dynamic programming over the last two rows
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define R 6                         /* number of columns */

/* Helper tables */
static unsigned char horiz[64][R];               /* left/right neighbours in a row */
static unsigned char diag[64][64][R];            /* diagonal neighbours from prev row */
static unsigned char partial[4096][R];           /* horiz + diag from prevPrev */
static unsigned char nxt[4096][64];              /* next rows for a (prevPrev,prev) state */
static unsigned char nxtcnt[4096];               /* number of valid next rows */
static bool precomp_done = false;

/* Pre‑compute tables – done once */
static void precompute(void)
{
    if (precomp_done) return;

    /* diagonal contributions: only up-left and up-right (no vertical) */
    for (int p = 0; p < 64; ++p)
        for (int c = 0; c < 64; ++c)
            for (int r = 0; r < R; ++r) {
                int v = 0;
                if (r > 0) v += (p >> (r-1)) & 1;
                if (r < R-1) v += (p >> (r+1)) & 1;
                diag[p][c][r] = (unsigned char)v;
            }

    /* horizontal neighbours inside a row */
    for (int mask = 0; mask < 64; ++mask)
        for (int r = 0; r < R; ++r) {
            int v = 0;
            if (r > 0) v += (mask >> (r-1)) & 1;
            if (r < R-1) v += (mask >> (r+1)) & 1;
            horiz[mask][r] = (unsigned char)v;
        }

    /* partial table: horiz + diagonal from prevPrev */
    for (int pp = 0; pp < 64; ++pp)
        for (int p = 0; p < 64; ++p) {
            int id = pp*64 + p;
            for (int r = 0; r < R; ++r)
                partial[id][r] = horiz[p][r] + diag[pp][p][r];
        }

    /* next patterns for each state (prevPrev,prev) */
    for (int pp = 0; pp < 64; ++pp)
        for (int p = 0; p < 64; ++p) {
            int id = pp*64 + p;
            int cnt = 0;
            for (int n = 0; n < 64; ++n) {
                bool ok = true;
                for (int r = 0; r < R && ok; ++r)
                    if ((p >> r) & 1) {                    /* 1 in prev row */
                        int tot = partial[id][r] + diag[n][p][r]; /* neigh. from prevPrev and next */
                        ok = (tot == 1 || tot == 5);
                    }
                if (ok) nxt[id][cnt++] = (unsigned char)n;
            }
            nxtcnt[id] = (unsigned char)cnt;
        }

    precomp_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    precompute();

    /* DP arrays: dp[prevPrev][prev] */
    static seq_t dp[64][64];
    static seq_t dp2[64][64];

    memset(dp, 0, sizeof(dp));
    for (int r = 0; r < 64; ++r)
        dp[0][r] = 1;               /* dummy previous row of zeros */

    for (idx_t row = 2; row <= n; ++row) {      /* transition for each new row */
        memset(dp2, 0, sizeof(dp2));

        for (int pp = 0; pp < 64; ++pp)
            for (int p = 0; p < 64; ++p) {
                seq_t ways = dp[pp][p];
                if (!ways) continue;

                int id = pp*64 + p;
                int cmax = nxtcnt[id];
                for (int k = 0; k < cmax; ++k) {
                    int n = nxt[id][k];
                    dp2[p][n] += ways;
                }
            }
        memcpy(dp, dp2, sizeof(dp));
    }

    /* Final row check */
    seq_t result = 0;
    for (int pp = 0; pp < 64; ++pp)
        for (int cur = 0; cur < 64; ++cur) {
            seq_t ways = dp[pp][cur];
            if (!ways) continue;

            int id = pp*64 + cur;
            bool ok = true;
            for (int r = 0; r < R; ++r)
                if ((cur >> r) & 1) {
                    int cnt = partial[id][r];        /* horiz + diag from prev */
                    if (!(cnt == 1 || cnt == 5)) { ok = false; break; }
                }
            if (ok) result += ways;
        }

    return result;
}
