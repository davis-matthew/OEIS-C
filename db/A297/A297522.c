/*
 * A297522  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over pairs of consecutive columns. Each 4×n binary matrix is
 *            built column by column. For any cell that contains a 1, the
 *            number of its adjacent 1s (horizontal, diagonal and antidiagonal
 *            – six possible neighbours) must be exactly 1 or 3. The DP keeps
 *            the pattern of the last two columns; when a...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool _ok[16][16][16] = {0};
static bool _ok_init = false;

/* Pre‑compute whether a column C satisfies the condition when
   its left neighbour runs as L and right neighbour as R.  */
static void init_ok(void) {
    if (_ok_init) return;
    for (uint8_t L = 0; L < 16; ++L) {
        for (uint8_t C = 0; C < 16; ++C) {
            for (uint8_t R = 0; R < 16; ++R) {
                bool good = true;
                for (int r = 0; r < 4; ++r) {
                    if (!(C & (1 << r))) continue;          /* cell is 0 */
                    int neigh = 0;
                    /* left/right */
                    if (L & (1 << r)) ++neigh;
                    if (R & (1 << r)) ++neigh;
                    /* up-left */
                    if (r > 0 && (L & (1 << (r - 1)))) ++neigh;
                    /* down-left */
                    if (r < 3 && (L & (1 << (r + 1)))) ++neigh;
                    /* up-right */
                    if (r > 0 && (R & (1 << (r - 1)))) ++neigh;
                    /* down-right */
                    if (r < 3 && (R & (1 << (r + 1)))) ++neigh;
                    if (neigh != 1 && neigh != 3) {
                        good = false;
                        break;
                    }
                }
                _ok[L][C][R] = good;
            }
        }
    }
    _ok_init = true;
}

seq_t a(idx_t n) {
    init_ok();

    /* dp[L][C] counts ways where the last two columns are L (previous)
       and C (current), and L has already satisfied its condition.
       C is still pending validation until the next column arrives. */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* initialise for n >= 1 */
    for (uint8_t i = 0; i < 16; ++i) {
        for (uint8_t j = 0; j < 16; ++j) {
            dp[i][j] = 0;
        }
    }
    for (uint8_t C = 0; C < 16; ++C) dp[0][C] = 1;   /* first column, left side zeros */

    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t C = 0; C < 16; ++C) {
            if (_ok[0][C][0]) ans += dp[0][C];
        }
        return ans;
    }

    for (idx_t col = 2; col <= n; ++col) {
        /* clear new DP */
        for (uint8_t i = 0; i < 16; ++i)
            for (uint8_t j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (uint8_t L = 0; L < 16; ++L) {
            for (uint8_t C = 0; C < 16; ++C) {
                seq_t cur = dp[L][C];
                if (!cur) continue;
                for (uint8_t R = 0; R < 16; ++R) {
                    if (_ok[L][C][R]) ndp[C][R] += cur;
                }
            }
        }
        /* move ndp -> dp */
        for (uint8_t i = 0; i < 16; ++i)
            for (uint8_t j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* validate last column with dummy zeros to the right */
    seq_t result = 0;
    for (uint8_t L = 0; L < 16; ++L) {
        for (uint8_t C = 0; C < 16; ++C) {
            if (!dp[L][C]) continue;
            if (_ok[L][C][0]) result += dp[L][C];
        }
    }
    return result;
}
