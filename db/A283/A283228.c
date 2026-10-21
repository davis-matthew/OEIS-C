/*
 * A283228  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors, with the exception of exactly
 *          two elements.
 * https://oeis.org/A283228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = number of n×4 binary arrays in which exactly two cells
   violate the rule that a 1 may not have more than two neighboring 1s
   (horizontal and vertical).  This implementation uses dynamic programming. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Pre‑compute the number of violations contributed by the middle
       row when the rows above, middle and below are given.  V[mid][up][down]
       counts the cells in 'mid' that would have >2 1‑neighbors if up and
       down are as specified. */
    static seq_t V[16][16][16];
    static bool init = false;
    if (!init) {
        for (int mid = 0; mid < 16; ++mid) {
            for (int up = 0; up < 16; ++up) {
                for (int down = 0; down < 16; ++down) {
                    int cnt = 0;
                    for (int c = 0; c < 4; ++c) {
                        if (!((mid >> c) & 1)) continue;        /* cell is 0 */
                        int neigh = 0;
                        if (c > 0) neigh += (mid >> (c - 1)) & 1;
                        if (c < 3) neigh += (mid >> (c + 1)) & 1;
                        neigh += (up >> c) & 1;
                        neigh += (down >> c) & 1;
                        if (neigh > 2) ++cnt;
                    }
                    V[mid][up][down] = (seq_t)cnt;
                }
            }
        }
        init = true;
    }

    /* dp[prev2][prev1][v] : number of ways to realise the rows processed so far
       where 'prev2' and 'prev1' are the patterns of the two most recent rows
       and 'v' is the cumulative number of violation cells counted (0..2). */
    seq_t dp[16][16][3] = {0};
    seq_t ndp[16][16][3] = {0};

    /* Initial state: first row only, treat the row above as all zeros. */
    for (int cur = 0; cur < 16; ++cur)
        dp[0][cur][0] = (seq_t)1;

    /* Process rows 1 .. n-1, accumulating violations. */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 3; ++k)
                    ndp[i][j][k] = 0;

        for (int up = 0; up < 16; ++up) {            /* row i-2 */
            for (int mid = 0; mid < 16; ++mid) {     /* row i-1 */
                for (int v = 0; v < 3; ++v) {
                    seq_t ways = dp[up][mid][v];
                    if (ways == 0) continue;
                    for (int cur = 0; cur < 16; ++cur) { /* row i */
                        int viol = (int)V[mid][up][cur];
                        if (v + viol > 2) continue;
                        ndp[mid][cur][v + viol] += ways;
                    }
                }
            }
        }

        /* Copy ndp to dp elementwise to avoid undefined behaviour with
           non‑trivially copyable seq_t objects. */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = ndp[i][j][k];
    }

    /* Final row: count violations with the imaginary zero row below. */
    seq_t ans = 0;
    for (int up = 0; up < 16; ++up) {        /* row n-2 */
        for (int last = 0; last < 16; ++last) { /* row n-1 */
            for (int v = 0; v < 3; ++v) {
                seq_t ways = dp[up][last][v];
                if (ways == 0) continue;
                int viol = (int)V[last][up][0];
                if (v + viol == 2) ans += ways;
            }
        }
    }

    return ans;
}
