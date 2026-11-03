/*
 * A284077  Number of 3Xn 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A284077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 202 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A284077 sequence: number of 3×n binary arrays with no 1 equal to
   more than three of its horizontal, diagonal, and antidiagonal neighbors,
   with the exception of exactly two elements.
   The DP below counts all such arrays exactly for arbitrary‑precision
   or fixed‑width integer types.
*/

#include <stdint.h>
#include <stdbool.h>

static int neigh_violations(int l, int m, int r)
{
    /* Count for each 1 in column m the number of 1‑valued neighbors
       among its horizontal, diagonal, and antidiagonal cells. */
    int tot = 0;
    for (int row = 0; row < 3; ++row) {
        if ((m >> row) & 1) {          /* cell is 1 */
            int n = 0;
            /* horizontal neighbors */
            if ((l >> row) & 1) n++;  /* left */
            if ((r >> row) & 1) n++;  /* right */

            /* diagonal neighbors (left side) */
            if (row > 0 && (l >> (row - 1)) & 1) n++;    /* left‑up */
            if (row < 2 && (l >> (row + 1)) & 1) n++;    /* left‑down */

            /* antidiagonal neighbors (right side) */
            if (row > 0 && (r >> (row - 1)) & 1) n++;    /* right‑up */
            if (row < 2 && (r >> (row + 1)) & 1) n++;    /* right‑down */

            if (n > 3) tot++;           /* violation of this 1 */
        }
    }
    return tot;
}

static void init_viol_table(int table[8][8][8])
{
    for (int l = 0; l < 8; ++l)
        for (int m = 0; m < 8; ++m)
            for (int r = 0; r < 8; ++r)
                table[l][m][r] = neigh_violations(l, m, r);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Pre‑compute the table of violations for all (l,m,r) combinations. */
    static int viol[8][8][8];
    static bool table_ready = false;
    if (!table_ready) { init_viol_table(viol); table_ready = true; }

    /* DP: dp[64][3] – 64 possible (l,m) states, with 0..2 violations seen. */
    seq_t dp[64][3];
    seq_t ndp[64][3];
    for (int i = 0; i < 64; ++i)
        for (int v = 0; v < 3; ++v)
            dp[i][v] = ndp[i][v] = (seq_t)0;

    /* Initial column: left boundary is all zeros. */
    for (int m = 0; m < 8; ++m)
        dp[(0 << 3) | m][0] = dp[(0 << 3) | m][0] + (seq_t)1;

    /* Process columns 1 … n‑1. */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < 64; ++i)
            for (int v = 0; v < 3; ++v)
                ndp[i][v] = (seq_t)0;

        for (int old = 0; old < 64; ++old) {
            int l = old >> 3;
            int m = old & 7;
            for (int v = 0; v < 3; ++v) {
                seq_t val = dp[old][v];
                if (val == (seq_t)0) continue;
                for (int r = 0; r < 8; ++r) {
                    int add = viol[l][m][r];
                    int nv = v + add;
                    if (nv <= 2) {
                        int nstate = (m << 3) | r;
                        ndp[nstate][nv] = ndp[nstate][nv] + val;
                    }
                }
            }
        }

        /* Copy ndp to dp for the next iteration. */
        for (int i = 0; i < 64; ++i)
            for (int v = 0; v < 3; ++v)
                dp[i][v] = ndp[i][v];
    }

    /* Final column check (right boundary is all zeros). */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < 64; ++s) {
        int l = s >> 3;
        int m = s & 7;
        for (int v = 0; v < 3; ++v) {
            seq_t val = dp[s][v];
            if (val == (seq_t)0) continue;
            int add = viol[l][m][0];
            int total = v + add;
            if (total == 2) ans = ans + val;
        }
    }
    return ans;
}
