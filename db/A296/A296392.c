/*
 * A296392  Number of nX4 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A296392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 0..1 arrays in which every 1 has exactly 1
 *            or exactly 3 neighboring 1s (adjacent horizontally, vertically
 *            or antidiagonally). The implementation uses a transfer‑matrix
 *            dynamic programme. The antidiagonal direction considered is the
 *            NE–SW diagonal (up‑right / down‑left). The r...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute whether a 4‑bit row “curr” is valid when the previous
   row is “prev” and the next row is “next”.  A row is valid if
   every 1 in “curr” has exactly one or exactly three neighbours
   among the eight possible directions (horizontally, vertically
   or antidiagonally, with the antidiagonal taken to be the
   NE‑SW direction only). */
static inline bool row_valid(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 4; ++j) {
        if (!(curr >> j & 1))
            continue;
        int nb = 0;
        /* horizontal neighbours */
        if (j > 0)  nb += (curr >> (j - 1)) & 1;
        if (j < 3)  nb += (curr >> (j + 1)) & 1;
        /* vertical neighbours */
        nb += (prev >> j) & 1;
        nb += (next >> j) & 1;
        /* antidiagonal neighbours (NE–SW direction) */
        if (j < 3)  nb += (prev >> (j + 1)) & 1;   /* up‑right  */
        if (j > 0)  nb += (next >> (j - 1)) & 1;   /* down‑left */
        if (!(nb == 1 || nb == 3))
            return false;
    }
    return true;
}

/* Main function as required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)                /* n ≥ 1 by definition; safe guard */
        return 0;

    /* DP arrays: dp[prev][curr] holds the number of ways to reach
       state (prev, curr) after processing the current row.  Only
       two rows of memory are needed. */
    seq_t dp0[16][16];
    seq_t dp1[16][16];
    seq_t (*dp)[16]  = dp0;
    seq_t (*ndp)[16] = dp1;

    /* initialise dp to zero */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j) {
            dp[i][j]  = 0;
            ndp[i][j] = 0;
        }

    /* first row: the upper neighbour is the zero mask */
    for (int curr = 0; curr < 16; ++curr)
        dp[0][curr] = 1;       /* prev = 0 */

    /* process subsequent rows */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev) {
            for (int curr = 0; curr < 16; ++curr) {
                seq_t cnt = dp[prev][curr];
                if (cnt == 0)
                    continue;
                for (int next = 0; next < 16; ++next) {
                    if (row_valid(prev, curr, next))
                        ndp[curr][next] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        seq_t (*tmp)[16] = dp;
        dp   = ndp;
        ndp  = tmp;
    }

    /* final validation with sentinel bottom row (mask 0) */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int curr = 0; curr < 16; ++curr) {
            if (row_valid(prev, curr, 0))
                ans += dp[prev][curr];
        }
    }
    return ans;
}
