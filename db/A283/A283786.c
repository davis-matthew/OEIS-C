/*
 * A283786  Number of 3Xn 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly two elements.
 * https://oeis.org/A283786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by transfer‑matrix DP.
   State: (prev column, current column, number of violations so far ≤ 2).
   Neighbours considered: horizontal left/right and the four diagonals;
   vertical neighbours are NOT counted.  Exactly two violations are required. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the number of violating cells in column 'b'
       given left column 'a' and right column 'c'.  V[a][b][c]
       counts only the cells in column b.  Each column is a 3‑bit
       pattern (rows 0..2).  A violation occurs when a "1" in
       column b has more than two neighbours that are "1".  Vertically
       adjacent cells are NOT counted. */
    static unsigned char V[8][8][8];
    static bool V_init = false;
    if (!V_init) {
        for (int a = 0; a < 8; ++a) {
            for (int b = 0; b < 8; ++b) {
                for (int c = 0; c < 8; ++c) {
                    unsigned char cnt = 0;
                    for (int r = 0; r < 3; ++r) {
                        if (!(b & (1 << r))) continue;          /* cell is 0 */
                        int neigh = 0;
                        /* horizontal neighbours */
                        if (a & (1 << r)) neigh++;              /* left  */
                        if (c & (1 << r)) neigh++;              /* right */
                        /* diagonal – left */
                        if (r > 0 && (a & (1 << (r - 1)))) neigh++;       /* up‑left   */
                        if (r < 2 && (a & (1 << (r + 1)))) neigh++;       /* down‑left */
                        /* diagonal – right */
                        if (r > 0 && (c & (1 << (r - 1)))) neigh++;       /* up‑right  */
                        if (r < 2 && (c & (1 << (r + 1)))) neigh++;       /* down‑right*/
                        if (neigh > 2) ++cnt;
                    }
                    V[a][b][c] = cnt;
                }
            }
        }
        V_init = true;
    }

    /* DP arrays: dp[2][64][3] – 2 alternating layers, 64 = 8*8
       of (prev column, current column) states, and 3 possible
       violation counters (0..2).  seq_t type is arbitrary‑precise
       or native unsigned integer as provided by the build. */
    static seq_t dp[2][64][3];
    int cur = 0, next = 1;

    /* initialise dp for the first (virtual left) column: prev = 0,
       curr = p0.  No violations have been counted yet. */
    for (int i = 0; i < 64; ++i)
        for (int k = 0; k < 3; ++k)
            dp[cur][i][k] = 0;
    for (int p0 = 0; p0 < 8; ++p0)
        dp[cur][0 * 8 + p0][0] = (seq_t)1;

    /* Process columns 1 .. n‑1: for each new column we count
       violations in the previous column (curr) using the new column
       as the right neighbour. */
    for (idx_t col = 1; col < n; ++col) {
        /* clear next DP layer */
        for (int i = 0; i < 64; ++i)
            for (int k = 0; k < 3; ++k)
                dp[next][i][k] = 0;
        /* transition */
        for (int prev = 0; prev < 8; ++prev) {
            for (int curr = 0; curr < 8; ++curr) {
                int base = prev * 8 + curr;
                for (int k = 0; k <= 2; ++k) {
                    seq_t val = dp[cur][base][k];
                    if (!val) continue;
                    for (int newc = 0; newc < 8; ++newc) {
                        unsigned char v = V[prev][curr][newc];
                        int newk = k + v;
                        if (newk <= 2)
                            dp[next][curr * 8 + newc][newk] += val;
                    }
                }
            }
        }
        /* swap layers */
        int t = cur; cur = next; next = t;
    }

    /* Final column: the right neighbour beyond the last column is 0. */
    seq_t ans = 0;
    for (int prev = 0; prev < 8; ++prev) {
        for (int curr = 0; curr < 8; ++curr) {
            int base = prev * 8 + curr;
            for (int k = 0; k <= 2; ++k) {
                seq_t val = dp[cur][base][k];
                if (!val) continue;
                if (k + V[prev][curr][0] == 2)
                    ans += val;
            }
        }
    }
    return ans;
}
