/*
 * A296596  Number of nX5 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1, 2 or 3 neighboring 1s.
 * https://oeis.org/A296596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count of n×5 0/1 arrays where every 1 has 1–3 neighbors
   in the 6 allowed directions (left, right, up, down, up‑right, down‑left).
   Uses DP over rows with state (previous row, current row). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define MASKS 32   // 5 bits

static bool valid[MASKS][MASKS][MASKS];
static uint8_t next_mask[MASKS][MASKS][MASKS];
static uint8_t next_cnt[MASKS][MASKS];
static bool initialized = false;

/* pre‑compute transition validity */
static void init(void)
{
    if (initialized) return;
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            int idx = 0;
            for (int c = 0; c < MASKS; ++c) {
                bool ok = true;
                for (int col = 0; col < 5 && ok; ++col) {
                    if ((b >> col) & 1) {
                        int cnt = 0;
                        if (col > 0 && (b >> (col - 1)) & 1) ++cnt;
                        if (col < 4 && (b >> (col + 1)) & 1) ++cnt;
                        if (a & (1 << col)) ++cnt;
                        if (c & (1 << col)) ++cnt;
                        if (col < 4 && (a & (1 << (col + 1)))) ++cnt;
                        if (col > 0 && (c & (1 << (col - 1)))) ++cnt;
                        if (cnt == 0 || cnt > 3) ok = false;
                    }
                }
                valid[a][b][c] = ok;
                if (ok) {
                    next_mask[a][b][idx] = (uint8_t)c;
                    ++idx;
                }
            }
            next_cnt[a][b] = (uint8_t)idx;
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    /* dp[cur][prev][cur] */
    seq_t dp[2][MASKS][MASKS];
    memset(dp, 0, sizeof(dp));

    /* start with both previous rows 0 */
    dp[0][0][0] = 1;

    int cur = 0, next = 1;
    for (idx_t i = 0; i < n; ++i) {
        memset(dp[next], 0, sizeof(dp[next]));
        for (int a = 0; a < MASKS; ++a) {
            for (int b = 0; b < MASKS; ++b) {
                if (!dp[cur][a][b]) continue;
                uint8_t cnt = next_cnt[a][b];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int c = next_mask[a][b][k];
                    dp[next][b][c] += dp[cur][a][b];
                }
            }
        }
        cur ^= 1; next ^= 1;
    }

    /* add final zero row to validate the last real row */
    seq_t ans = 0;
    for (int a = 0; a < MASKS; ++a) {
        for (int b = 0; b < MASKS; ++b) {
            if (valid[a][b][0]) ans += dp[cur][a][b];
        }
    }
    return ans;
}
