/*
 * A296113  Number of nX6 0..1 arrays with each 1 adjacent to 3 or 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A296113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP algorithm for A296113: number of n×6 0–1 arrays with each 1
 *            adjacent to 3 or 4 king‑move neighbouring 1s
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t pre_mask[64][64];
static uint8_t  pre_final[64][64];
static int     pre_init = 0;

static void
precompute(void)
{
    if (pre_init) return;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            for (int c = 0; c < 64; ++c) {
                int ok = 1;
                for (int j = 0; j < 6 && ok; ++j) if ((b >> j) & 1) {
                    int cnt = 0;
                    for (int dy = -1; dy <= 1 && ok; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            if (dy == 0 && dx == 0) continue;
                            int jj = j + dx;
                            if (jj < 0 || jj >= 6) continue;
                            if (dy == -1 && ((a >> jj) & 1)) ++cnt;
                            if (dy ==  0 && ((b >> jj) & 1)) ++cnt;
                            if (dy ==  1 && ((c >> jj) & 1)) ++cnt;
                        }
                    }
                    if (cnt != 3 && cnt != 4) ok = 0;
                }
                if (ok) {
                    pre_mask[a][b] |= 1ULL << c;
                    if (c == 0) pre_final[a][b] = 1;
                }
            }
        }
    }
    pre_init = 1;
}

seq_t
a(idx_t n)
{
    precompute();

    static seq_t dp[2][64][64];
    int cur = 0, nxt = 1;

    /* initialize dp */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            dp[cur][i][j] = 0;
    dp[cur][0][0] = (seq_t)1;

    for (idx_t r = 1; r <= n; ++r) {
        /* zero next dp */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[nxt][i][j] = 0;

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t curVal = dp[cur][a][b];
                if (curVal == 0) continue;
                uint64_t mask = pre_mask[a][b];
                while (mask) {
                    uint64_t low = mask & -mask;
                    int c = __builtin_ctzll(mask);
                    dp[nxt][b][c] += curVal;
                    mask ^= low;
                }
            }
        }
        cur ^= 1; nxt ^= 1;
    }

    seq_t ans = 0;
    for (int a = 0; a < 64; ++a) {
        for (int b = 0; b < 64; ++b) {
            if (pre_final[a][b]) ans += dp[cur][a][b];
        }
    }
    return ans;
}
