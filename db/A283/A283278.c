/*
 * A283278  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which no 1 has more than
 *            two of its horizontal, vertical, or antidiagonal neighbors
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming on 4‑bit row
   patterns.  For each pair of successive rows we precompute which
   next rows are allowed, then apply the transition for all n rows. */

#include <stdbool.h>

/* The types seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; a(0) is undefined */
        return 0;

    static bool built = false;
    static bool valid[16][16][16];
    static bool last_valid[16][16];

    if (!built) {
        /* ---- Precompute transition tables ---- */
        for (int p2 = 0; p2 < 16; ++p2)
            for (int p1 = 0; p1 < 16; ++p1)
                for (int cur = 0; cur < 16; ++cur) {
                    bool ok = true;
                    for (int j = 0; j < 4 && ok; ++j) {
                        if (!((p1 >> j) & 1)) continue;   /* cell is 0 */
                        int cnt = 0;
                        /* horizontal neighbors */
                        if (j > 0 && ((p1 >> (j - 1)) & 1)) ++cnt;
                        if (j < 3 && ((p1 >> (j + 1)) & 1)) ++cnt;
                        /* vertical neighbors */
                        if ((p2 >> j) & 1) ++cnt;          /* up */
                        if ((cur >> j) & 1) ++cnt;          /* down */
                        /* antidiagonal neighbors */
                        if (j < 3 && ((p2 >> (j + 1)) & 1)) ++cnt; /* up‑right */
                        if (j > 0 && ((cur >> (j - 1)) & 1)) ++cnt;/* down‑left */
                        if (cnt > 2) ok = false;
                    }
                    valid[p2][p1][cur] = ok;
                }

        for (int p2 = 0; p2 < 16; ++p2)
            for (int p1 = 0; p1 < 16; ++p1) {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if (!((p1 >> j) & 1)) continue;
                    int cnt = 0;
                    /* horizontal neighbors */
                    if (j > 0 && ((p1 >> (j - 1)) & 1)) ++cnt;
                    if (j < 3 && ((p1 >> (j + 1)) & 1)) ++cnt;
                    /* vertical neighbors */
                    if ((p2 >> j) & 1) ++cnt;          /* up */
                    /* next row is all zeros – no down neighbor */
                    /* antidiagonal neighbor up‑right */
                    if (j < 3 && ((p2 >> (j + 1)) & 1)) ++cnt;
                    /* down‑left is zero */
                    if (cnt > 2) ok = false;
                }
                last_valid[p2][p1] = ok;
            }
        built = true;
    }

    /* ---- Dynamic programming over rows ---- */
    seq_t dp[16][16] = {0};
    seq_t newdp[16][16];

    /* First row: previous outside row (index 0) is all zeros. */
    for (int r = 0; r < 16; ++r)
        dp[0][r] = 1;

    if (n == 1) {
        seq_t sum = 0;
        for (int p1 = 0; p1 < 16; ++p1)
            if (last_valid[0][p1])
                sum += dp[0][p1];
        return sum;
    }

    for (idx_t row = 2; row <= n; ++row) {
        /* reset newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        for (int p2 = 0; p2 < 16; ++p2)
            for (int p1 = 0; p1 < 16; ++p1) {
                seq_t val = dp[p2][p1];
                if (!val) continue;
                for (int cur = 0; cur < 16; ++cur)
                    if (valid[p2][p1][cur])
                        newdp[p1][cur] += val;
            }

        /* copy newdp back to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final validation of the bottom row */
    seq_t total = 0;
    for (int p2 = 0; p2 < 16; ++p2)
        for (int p1 = 0; p1 < 16; ++p1)
            if (last_valid[p2][p1])
                total += dp[p2][p1];

    return total;
}
