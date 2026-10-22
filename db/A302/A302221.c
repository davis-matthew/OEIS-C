/*
 * A302221  Number of n X 5 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP over n×5 binary arrays with adjacency constraints
   (each cell has 1–3 equal neighbors among horizontal and diagonal
   adjacencies).  The upper‑left element is fixed to 0.
   Complexity: O(n). */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline bool check_row(uint8_t cur, uint8_t up, uint8_t down,
                             bool has_up, bool has_down)
{
    /* Count equal neighbors from left/right and the four diagonals.
       No direct vertical adjacency is counted. */
    for (int j = 0; j < 5; ++j) {
        int v = (cur >> j) & 1;
        int cnt = 0;
        if (j > 0 && ((cur >> (j - 1)) & 1) == v) cnt++;      /* left   */
        if (j < 4 && ((cur >> (j + 1)) & 1) == v) cnt++;      /* right  */
        if (has_up) {
            if (j > 0   && ((up >> (j - 1)) & 1) == v) cnt++; /* up‑left   */
            if (j < 4   && ((up >> (j + 1)) & 1) == v) cnt++; /* up‑right  */
        }
        if (has_down) {
            if (j > 0   && ((down >> (j - 1)) & 1) == v) cnt++; /* down‑left */
            if (j < 4   && ((down >> (j + 1)) & 1) == v) cnt++; /* down‑right*/
        }
        if (cnt < 1 || cnt > 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static bool validTop[32][32];
    static bool validBottom[32][32];
    static bool validMid[32][32][32];

    /* Pre‑compute validity tables once */
    if (!initialized) {
        for (uint8_t p1 = 0; p1 < 32; ++p1) {
            for (uint8_t p2 = 0; p2 < 32; ++p2) {
                validTop[p1][p2]     = check_row(p1, 0, p2, false, true);
                validBottom[p1][p2]  = check_row(p2, p1, 0, true, false);
                for (uint8_t p3 = 0; p3 < 32; ++p3) {
                    validMid[p1][p2][p3] = check_row(p2, p1, p3, true, true);
                }
            }
        }
        initialized = true;
    }

    if (n == 0) return 0;

    /* n == 1: single row */
    if (n == 1) {
        seq_t ans = 0;
        for (uint8_t r = 0; r < 32; ++r) {
            if (r & 1) continue;               /* upper‑left must be 0 */
            if (check_row(r, 0, 0, false, false))
                ans += (seq_t)1;
        }
        return ans;
    }

    static seq_t dp[32][32];
    static seq_t dp_next[32][32];

    /* Initialize dp with rows 1 and 2 */
    memset(dp, 0, sizeof dp);
    for (int p1 = 0; p1 < 32; ++p1) {
        if (p1 & 1) continue;                   /* upper‑left is 0 */
        for (int p2 = 0; p2 < 32; ++p2) {
            if (validTop[p1][p2]) dp[p1][p2] = (seq_t)1;
        }
    }

    /* Process rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        memset(dp_next, 0, sizeof dp_next);
        for (int p1 = 0; p1 < 32; ++p1) {
            for (int p2 = 0; p2 < 32; ++p2) {
                seq_t ways = dp[p1][p2];
                if (!ways) continue;
                for (int p3 = 0; p3 < 32; ++p3) {
                    if (validMid[p1][p2][p3])
                        dp_next[p2][p3] += ways;
                }
            }
        }
        memcpy(dp, dp_next, sizeof dp);
    }

    /* Count valid completions for the final row */
    seq_t ans = 0;
    for (int p1 = 0; p1 < 32; ++p1) {
        for (int p2 = 0; p2 < 32; ++p2) {
            if (dp[p1][p2] && validBottom[p1][p2])
                ans += dp[p1][p2];
        }
    }
    return ans;
}
