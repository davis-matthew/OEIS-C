/*
 * A282476  Number of nX3 0..1 arrays with no 1 equal to more than three of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows with two previous rows,
   counting violations of the king‑move neighbor rule.  Exactly two
   violations are allowed. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute violations for all triples of rows.  viol[pp][p][c]
       is the number of 1s in row p that have more than three 1‑neighbors
       when the row above is pp and the row below is c. */
    static unsigned char viol[8][8][8];
    static bool initialized = false;
    if (!initialized) {
        for (int pp = 0; pp < 8; ++pp) {
            for (int p  = 0; p < 8; ++p) {
                for (int c  = 0; c < 8; ++c) {
                    unsigned char v = 0;
                    for (int col = 0; col < 3; ++col) {
                        if ((p >> col) & 1) {
                            int neigh = 0;
                            /* neighbors from the row above (pp) */
                            for (int d = -1; d <= 1; ++d) {
                                int idx = col + d;
                                if (idx >= 0 && idx < 3)
                                    neigh += (pp >> idx) & 1;
                            }
                            /* neighbors from the row below (c) */
                            for (int d = -1; d <= 1; ++d) {
                                int idx = col + d;
                                if (idx >= 0 && idx < 3)
                                    neigh += (c >> idx) & 1;
                            }
                            /* same row left/right neighbors */
                            if (col > 0) neigh += (p >> (col - 1)) & 1;
                            if (col < 2) neigh += (p >> (col + 1)) & 1;
                            if (neigh > 3) ++v;
                        }
                    }
                    viol[pp][p][c] = v;
                }
            }
        }
        initialized = true;
    }

    /* DP: dp[pp][p][v] is the number of ways to build the processed
       rows with last two rows equal to pp and p, with exactly v
       violations so far (0 ≤ v ≤ 2). */
    seq_t dp[8][8][3] = {0};
    seq_t ndp[8][8][3] = {0};
    dp[0][0][0] = 1;          /* start with two dummy zero rows */

    for (idx_t i = 0; i < n; ++i) {
        /* reset ndp */
        for (int pp = 0; pp < 8; ++pp)
            for (int p = 0; p < 8; ++p)
                for (int v = 0; v < 3; ++v)
                    ndp[pp][p][v] = 0;

        for (int pp = 0; pp < 8; ++pp) {
            for (int p = 0; p < 8; ++p) {
                for (int v = 0; v < 3; ++v) {
                    seq_t curcnt = dp[pp][p][v];
                    if (!curcnt) continue;
                    for (int c = 0; c < 8; ++c) {
                        int nv = v + viol[pp][p][c];
                        if (nv > 2) continue;
                        ndp[p][c][nv] = ndp[p][c][nv] + curcnt;
                    }
                }
            }
        }

        /* copy ndp into dp for next iteration */
        for (int pp = 0; pp < 8; ++pp)
            for (int p = 0; p < 8; ++p)
                for (int v = 0; v < 3; ++v)
                    dp[pp][p][v] = ndp[pp][p][v];
    }

    /* Finalize constraints for the last row (p) with a dummy zero row below. */
    seq_t ans = 0;
    for (int pp = 0; pp < 8; ++pp) {
        for (int p = 0; p < 8; ++p) {
            for (int v = 0; v < 3; ++v) {
                seq_t curcnt = dp[pp][p][v];
                if (!curcnt) continue;
                int finalv = v + viol[pp][p][0];
                if (finalv == 2)
                    ans = ans + curcnt;
            }
        }
    }
    return ans;
}
