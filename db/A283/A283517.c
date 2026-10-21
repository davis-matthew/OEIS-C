/*
 * A283517  Number of nX3 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×3 binary arrays with exactly two 1‑cells *
 *            that have more than three 1 neighbors among horizontal, vertical
 *            * and antidiagonal positions. * The function counts all such
 *            arrays for any n≥1. It uses a dynamic * programming over rows
 *            with state ⟨prevrow, prevprevrow, violations⟩;...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Precompute the violation count for each trio of rows.
     * viol_table[pp][p][cur] = number of 1‑cells in row 'p' that have
     * more than three neighboring 1‑cells among rows pp, p, cur.
     */
    static unsigned char viol_table[8][8][8];
    static int table_initialized = 0;
    if (!table_initialized) {
        for (int pp = 0; pp < 8; ++pp) {
            for (int p  = 0; p  < 8; ++p) {
                for (int cur = 0; cur < 8; ++cur) {
                    unsigned char cnt = 0;
                    for (int j = 0; j < 3; ++j) {
                        if ((p >> j) & 1U) {            /* cell is 1 */
                            int neigh = 0;
                            /* horizontal */
                            if (j > 0)   neigh += (p >> (j-1)) & 1U;
                            if (j < 2)   neigh += (p >> (j+1)) & 1U;
                            /* vertical */
                            neigh += (pp >> j) & 1U;      /* up */
                            neigh += (cur >> j) & 1U;     /* down */
                            /* antidiagonal */
                            if (j < 2)   neigh += (pp >> (j+1)) & 1U;  /* up‑right */
                            if (j > 0)   neigh += (cur >> (j-1)) & 1U;  /* down‑left */
                            if (neigh > 3) ++cnt;
                        }
                    }
                    viol_table[pp][p][cur] = cnt;
                }
            }
        }
        table_initialized = 1;
    }

    /* DP tables: dp[prevprev][prev][violations] */
    seq_t dp[8][8][3];
    seq_t newdp[8][8][3];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            for (int k = 0; k < 3; ++k)
                dp[i][j][k] = 0;

    dp[0][0][0] = 1;          /* start with two dummy zero rows */

    for (idx_t row = 1; row <= n; ++row) {
        /* zero newdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    newdp[i][j][k] = 0;

        for (int pp = 0; pp < 8; ++pp) {
            for (int p = 0; p < 8; ++p) {
                for (int v = 0; v < 3; ++v) {
                    seq_t ways = dp[pp][p][v];
                    if (!ways) continue;
                    /* try all possible next rows */
                    for (int cur = 0; cur < 8; ++cur) {
                        int add = 0;
                        if (row >= 2)                  /* at least one real row below */
                            add = viol_table[pp][p][cur];
                        int nv = v + add;
                        if (nv > 2) continue;
                        newdp[p][cur][nv] += ways;
                    }
                }
            }
        }
        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                for (int k = 0; k < 3; ++k)
                    dp[i][j][k] = newdp[i][j][k];
    }

    /* After all rows processed, count violations in the last row
     * using a dummy zero row after the last real row.
     */
    seq_t ans = 0;
    for (int pp = 0; pp < 8; ++pp) {
        for (int p = 0; p < 8; ++p) {
            for (int v = 0; v < 3; ++v) {
                seq_t ways = dp[pp][p][v];
                if (!ways) continue;
                int last = viol_table[pp][p][0];   /* cur = 0 dummy */
                if (v + last == 2)
                    ans += ways;
            }
        }
    }
    return ans;
}
