/*
 * A297639  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A297639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 3×n 0‑1 arrays where each cell that is 1 has 0, 2 or
 *            3 neighboring 1's among horizontally or diagonally adjacent
 *            cells (no vertical adjacency is considered). The implementation
 *            uses a transfer‑matrix dynamic programming over all 8 possible
 *            column patterns
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* check whether column `b` satisfies the adjacency constraint
   given its left neighbor pattern `a` and right neighbor pattern `c` */
static inline bool valid(int a, int b, int c)
{
    for (int r = 0; r < 3; ++r) {
        if ((b >> r) & 1) {
            int cnt = 0;
            /* neighbors from the left column `a` */
            cnt += (a >> r) & 1;                 /* same row */
            if (r > 0)   cnt += (a >> (r - 1)) & 1; /* up‑left */
            if (r < 2)   cnt += (a >> (r + 1)) & 1; /* down‑left */
            /* neighbors from the right column `c` */
            cnt += (c >> r) & 1;                 /* same row */
            if (r > 0)   cnt += (c >> (r - 1)) & 1; /* up‑right */
            if (r < 2)   cnt += (c >> (r + 1)) & 1; /* down‑right */
            if (cnt == 1 || cnt > 3) return false;   /* forbidden counts */
        }
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1 */
    if (n == 0) return 0;

    /* dp[p][c] = number of ways for the last two columns to be
       pattern p (col i‑1) and c (col i).  Only 8 patterns exist. */
    seq_t dp[8][8] = {0};
    seq_t next[8][8];

    /* first column: left neighbor is all zeros (pattern 0) */
    for (int c = 0; c < 8; ++c) dp[0][c] = 1;

    /* iterate over the remaining columns */
    for (idx_t col = 1; col < n; ++col) {
        /* clear next */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                next[i][j] = 0;

        /* transition from dp[p][c] to next[c][t] */
        for (int p = 0; p < 8; ++p) {
            for (int c = 0; c < 8; ++c) {
                seq_t ways = dp[p][c];
                if (!ways) continue;
                for (int t = 0; t < 8; ++t) {
                    if (valid(p, c, t))
                        next[c][t] += ways;
                }
            }
        }
        /* copy next to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = next[i][j];
    }

    /* last column: right neighbor is all zeros (pattern 0) */
    seq_t ans = 0;
    for (int p = 0; p < 8; ++p) {
        for (int c = 0; c < 8; ++c) {
            seq_t ways = dp[p][c];
            if (!ways) continue;
            if (valid(p, c, 0))
                ans += ways;
        }
    }
    return ans;
}
