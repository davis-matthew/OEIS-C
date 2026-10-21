/*
 * A297523  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 3 neighboring 1s.
 * https://oeis.org/A297523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A297523: number of 5×n binary arrays in which every 1 has
   either 1 or 3 neighboring 1s among the 8 possible directions.
   The value is obtained by a dynamic‑programming over columns
   (state = pair of consecutive columns).  The transition graph is
   pre‑computed once.  The routine is correct for any seq_t and
   idx_t supplied by the build. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define M 32                               /* 2^5 column patterns */

/* Check whether a column 'curr' satisfies the adjacency rule
   given left neighbor 'prev' and right neighbor 'next'.  Each
   argument is a 5‑bit mask (bit r corresponds to row r). */
static bool valid_col(int prev, int curr, int next)
{
    for (int r = 0; r < 5; ++r) {
        if ((curr >> r) & 1) {                /* cell (r,curr) is 1   */
            int neigh = 0;
            /* left neighbour */
            neigh += (prev >> r) & 1;
            if (r > 0)   neigh += (prev >> (r - 1)) & 1;
            if (r < 4)   neigh += (prev >> (r + 1)) & 1;
            /* right neighbour */
            neigh += (next >> r) & 1;
            if (r > 0)   neigh += (next >> (r - 1)) & 1;
            if (r < 4)   neigh += (next >> (r + 1)) & 1;
            if (neigh != 1 && neigh != 3) return false;
        }
    }
    return true;
}

/* Dynamic‑programming over columns.  States are pairs of column masks
   (previous, current).  dp[p][c] holds the number of ways to fill the
   processed part ending with the column pair (p,c). */
seq_t a(idx_t n)
{
    static seq_t dp[M][M], ndp[M][M];
    static uint8_t next_list[M][M][M];
    static unsigned short next_cnt[M][M];
    static bool prepared = false;

    /* Pre‑compute admissible next columns for each (prev,curr). */
    if (!prepared) {
        for (int prev = 0; prev < M; ++prev) {
            for (int curr = 0; curr < M; ++curr) {
                unsigned short cnt = 0;
                for (int nxt = 0; nxt < M; ++nxt) {
                    if (valid_col(prev, curr, nxt)) {
                        next_list[prev][curr][cnt++] = (uint8_t)nxt;
                    }
                }
                next_cnt[prev][curr] = cnt;
            }
        }
        prepared = true;
    }

    /* Reset DP tables. */
    memset(dp, 0, sizeof(dp));
    memset(ndp, 0, sizeof(ndp));

    /* === case n = 1 === */
    if (n == 1) {
        seq_t ans = 0;
        for (int col = 0; col < M; ++col)
            if (valid_col(0, col, 0))
                ans += 1;
        return ans;
    }

    /* === initialisation for the first two columns (col 1,2) === */
    for (int first = 0; first < M; ++first)
        for (int second = 0; second < M; ++second)
            if (valid_col(0, first, second))
                dp[first][second] = 1;

    /* Process columns 3 … n */
    for (idx_t col = 3; col <= n; ++col) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < M; ++prev) {
            for (int curr = 0; curr < M; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                unsigned short cnt = next_cnt[prev][curr];
                for (unsigned short k = 0; k < cnt; ++k) {
                    int nxt = next_list[prev][curr][k];
                    ndp[curr][nxt] += ways;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* === final column validation (right neighbour = 0) === */
    seq_t ans = 0;
    for (int prev = 0; prev < M; ++prev)
        for (int curr = 0; curr < M; ++curr)
            if (valid_col(prev, curr, 0))
                ans += dp[prev][curr];

    return ans;
}
