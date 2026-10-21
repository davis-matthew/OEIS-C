/*
 * A297099  Number of nX5 0..1 arrays with no 1 adjacent to 4 king-move
 *          neighboring 1s.
 * https://oeis.org/A297099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays with no 1 adjacent to exactly
 *            four king‑move neighboring 1s
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   is_row_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_row_valid.h"

/* Precomputed transitions: trans[up][mid] gives a bitmask of all nxt for which
   (up,mid,nxt) satisfies the local constraint. */
static uint32_t trans[32][32];
static bool trans_done = false;

static void build_trans(void)
{
    for (uint32_t up = 0; up < 32; ++up) {
        for (uint32_t mid = 0; mid < 32; ++mid) {
            uint32_t mask = 0;
            for (uint32_t nxt = 0; nxt < 32; ++nxt) {
                if (is_row_valid(up, mid, nxt))
                    mask |= (1U << nxt);
            }
            trans[up][mid] = mask;
        }
    }
    trans_done = true;
}

seq_t a(idx_t n)
{
    if (!trans_done)
        build_trans();

    /* DP tables: dp[prev][curr] = number of ways to assign rows processed so far,
       ending with rows prev (row i‑1) and curr (row i). */
    static seq_t dp[32][32];
    static seq_t newdp[32][32];

    /* clear dp */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = 0;

    /* initial state: prev = 0 (empty), curr = r for every possible first row */
    for (uint32_t r = 0; r < 32; ++r)
        dp[0][r] = 1;

    /* process remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* clear newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int up = 0; up < 32; ++up) {
            for (int mid = 0; mid < 32; ++mid) {
                seq_t cur = dp[up][mid];
                if (cur == 0) continue;
                uint32_t mask = trans[up][mid];
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if ((mask >> nxt) & 1U)
                        newdp[mid][nxt] += cur;
                }
            }
        }

        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final boundary: down = 0 */
    seq_t ans = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            seq_t cur = dp[prev][curr];
            if (cur == 0) continue;
            if (is_row_valid(prev, curr, 0))
                ans += cur;
        }
    }
    return ans;
}
