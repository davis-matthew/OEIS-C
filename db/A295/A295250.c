/*
 * A295250  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1 or 3 1s.
 * https://oeis.org/A295250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary matrices where each 1 has 0, 1, or 3
 *            neighboring 1s (adjacency count ≠ 2, 4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Pre–compute a table good[up][cur][down] that tells whether
   a row cur is valid when its up and down neighbours are
   the rows up and down, respectively.  */
static bool good[32][32][32];
static bool good_computed = false;

static void precompute_good(void)
{
    if (good_computed) return;
    for (int up = 0; up < 32; ++up) {
        for (int cur = 0; cur < 32; ++cur) {
            for (int down = 0; down < 32; ++down) {
                bool ok = true;
                for (int b = 0; b < 5; ++b) {
                    if (cur & (1 << b)) {
                        int cnt = 0;
                        if (b > 0 && (cur & (1 << (b - 1)))) ++cnt;
                        if (b < 4 && (cur & (1 << (b + 1)))) ++cnt;
                        if (up  & (1 << b)) ++cnt;
                        if (down & (1 << b)) ++cnt;
                        if (cnt == 2 || cnt == 4) { ok = false; break; }
                    }
                }
                good[up][cur][down] = ok;
            }
        }
    }
    good_computed = true;
}

seq_t a(idx_t n)
{
    precompute_good();

    static seq_t dp[32][32];
    static seq_t ndp[32][32];

    /* initialise for the first row: up‐neighbour is 0 */
    memset(dp, 0, sizeof(dp));
    for (int m = 0; m < 32; ++m) dp[m][0] = 1;

    for (idx_t row = 2; row <= n; ++row) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < 32; ++prev) {
            for (int prevprev = 0; prevprev < 32; ++prevprev) {
                seq_t ways = dp[prev][prevprev];
                if (!ways) continue;
                for (int cur = 0; cur < 32; ++cur) {
                    if (!good[prevprev][prev][cur]) continue;
                    ndp[cur][prev] += ways;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    seq_t ans = 0;
    for (int last = 0; last < 32; ++last) {
        for (int second = 0; second < 32; ++second) {
            seq_t ways = dp[last][second];
            if (!ways) continue;
            if (good[second][last][0]) ans += ways;
        }
    }
    return ans;
}
