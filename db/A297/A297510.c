/*
 * A297510  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: dynamic programming over column masks.  
   For a 5×n binary grid, a 1 is valid only if it has 1 or 2
   adjacencies among the six horizontal/diagonal positions:
   left same row, left‑up, left‑down, right same row,
   right‑up, right‑down.  Vertical neighbours are NOT counted.
   The DP keeps the last two column masks and, when a new column
   is added, validates the middle column. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COL_MASK_COUNT 32          /* 2^5 possible column patterns */

/* Tables generated once. */
static unsigned char ok[COL_MASK_COUNT][COL_MASK_COUNT][COL_MASK_COUNT];
static uint32_t      nextMask[COL_MASK_COUNT][COL_MASK_COUNT];
static bool          ok_init = false;

/* Pre‑compute validity of three consecutive columns. */
static void init_ok(void)
{
    if (ok_init) return;
    ok_init = true;

    for (int left = 0; left < COL_MASK_COUNT; ++left) {
        for (int cur = 0; cur < COL_MASK_COUNT; ++cur) {
            for (int right = 0; right < COL_MASK_COUNT; ++right) {
                bool good = true;
                for (int r = 0; r < 5; ++r) {
                    if (!(cur & (1 << r))) continue;   /* cell is 0 */

                    int cnt = 0;

                    /* left neighbours */
                    if (left & (1 << r))          ++cnt;
                    if (r > 0  && (left & (1 << (r - 1)))) ++cnt;
                    if (r < 4  && (left & (1 << (r + 1)))) ++cnt;

                    /* right neighbours */
                    if (right & (1 << r))          ++cnt;
                    if (r > 0  && (right & (1 << (r - 1)))) ++cnt;
                    if (r < 4  && (right & (1 << (r + 1)))) ++cnt;

                    /* a 1 must have 1 or 2 neighbours */
                    if (cnt == 0 || cnt > 2) {
                        good = false;
                        break;
                    }
                }
                ok[left][cur][right] = good ? 1 : 0;
            }
        }
    }

    /* For each pair (left,cur) pre‑compute bitset of allowed next masks. */
    for (int left = 0; left < COL_MASK_COUNT; ++left) {
        for (int cur = 0; cur < COL_MASK_COUNT; ++cur) {
            uint32_t mask = 0;
            for (int nxt = 0; nxt < COL_MASK_COUNT; ++nxt)
                if (ok[left][cur][nxt]) mask |= (1u << nxt);
            nextMask[left][cur] = mask;
        }
    }
}

/* DP arrays: dp[prev][cur] holds counts for processed columns so far. */
static seq_t dp[COL_MASK_COUNT][COL_MASK_COUNT];
static seq_t dp_next[COL_MASK_COUNT][COL_MASK_COUNT];

seq_t a(idx_t n)
{
    init_ok();

    if (n == 0) return 0;          /* below offset: undefined */

    /* n == 1: only first column, left and right neighbours are 0. */
    if (n == 1) {
        seq_t res = 0;
        for (int b = 0; b < COL_MASK_COUNT; ++b)
            if (ok[0][b][0]) res += 1;
        return res;
    }

    /* Initialise dp with the first two columns.  Column 0 has left mask 0. */
    memset(dp, 0, sizeof(dp));
    for (int col0 = 0; col0 < COL_MASK_COUNT; ++col0) {
        for (int col1 = 0; col1 < COL_MASK_COUNT; ++col1) {
            if (ok[0][col0][col1]) dp[col0][col1] = 1;
        }
    }

    /* Process columns 3..n.  step counts number of columns already placed. */
    for (idx_t step = 2; step < n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < COL_MASK_COUNT; ++prev) {
            for (int cur = 0; cur < COL_MASK_COUNT; ++cur) {
                seq_t val = dp[prev][cur];
                if (!val) continue;
                /* try all allowed next masks */
                for (int nxt = 0; nxt < COL_MASK_COUNT; ++nxt) {
                    if (ok[prev][cur][nxt]) {
                        dp_next[cur][nxt] += val;
                    }
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* Validate the final column (right neighbour missing, i.e. 0). */
    seq_t ans = 0;
    for (int prev = 0; prev < COL_MASK_COUNT; ++prev) {
        for (int cur = 0; cur < COL_MASK_COUNT; ++cur) {
            if (dp[prev][cur] && ok[prev][cur][0]) {
                ans += dp[prev][cur];
            }
        }
    }
    return ans;
}
