/*
 * A297678  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 4 neighboring 1s.
 * https://oeis.org/A297678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by dynamic programming over rows with adjacency
 *            constraints Every 1 in an n×4 binary array must have a
 *            horizontal or diagonally adjacent 1 counted in its neighborhood
 *            of 6 possible neighbors, and the number of such neighbors must
 *            be 0, 1, or 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* unnecessary for offset 1, but harmless */

    /* DP state: dp[pv][v] counts ways where the last two processed rows are
       pv (row i‑1) and v (row i).  Row indices 0‑3 correspond to columns. */
    static seq_t dp[16][16];
    static seq_t newdp[16][16];
    /* initialize all to zero */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* initial row: treat imaginary previous row as all zeros */
    for (int cur = 0; cur < 16; ++cur)
        dp[0][cur] = 1;

    /* iterate from second row up to row n */
    for (idx_t row = 2; row <= n; ++row)
    {
        /* clear newdp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                newdp[i][j] = 0;

        /* transition */
        for (int prevprev = 0; prevprev < 16; ++prevprev)
        {
            for (int prev = 0; prev < 16; ++prev)
            {
                seq_t ways = dp[prevprev][prev];
                if (!ways) continue;

                for (int next = 0; next < 16; ++next)
                {
                    /* validate that all 1‑cells in row ‘prev’ satisfy the rule
                       once we add the new row ‘next’. */
                    int ok = 1;
                    for (int col = 0; col < 4 && ok; ++col)
                    {
                        if (!(prev & (1u << col))) continue;

                        int horiz = 0;
                        if (col > 0 && (prev & (1u << (col-1)))) horiz++;
                        if (col < 3 && (prev & (1u << (col+1)))) horiz++;

                        int up = 0;
                        if (col > 0 && (prevprev & (1u << (col-1)))) up++;
                        if (col < 3 && (prevprev & (1u << (col+1)))) up++;

                        int down = 0;
                        if (col > 0 && (next & (1u << (col-1)))) down++;
                        if (col < 3 && (next & (1u << (col+1)))) down++;

                        int deg = horiz + up + down;
                        if (!(deg == 0 || deg == 1 || deg == 4))
                            ok = 0;
                    }
                    if (ok)
                        newdp[prev][next] += ways;
                }
            }
        }

        /* copy results back to dp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* final step: finalize the last row (row n) with no succeeding row */
    seq_t total = 0;
    for (int prevprev = 0; prevprev < 16; ++prevprev)
    {
        for (int prev = 0; prev < 16; ++prev)
        {
            seq_t ways = dp[prevprev][prev];
            if (!ways) continue;

            int ok = 1;
            for (int col = 0; col < 4 && ok; ++col)
            {
                if (!(prev & (1u << col))) continue;

                int horiz = 0;
                if (col > 0 && (prev & (1u << (col-1)))) horiz++;
                if (col < 3 && (prev & (1u << (col+1)))) horiz++;

                int up = 0;
                if (col > 0 && (prevprev & (1u << (col-1)))) up++;
                if (col < 3 && (prevprev & (1u << (col+1)))) up++;

                /* no down neighbors for the last row */
                int deg = horiz + up;
                if (!(deg == 0 || deg == 1 || deg == 4))
                    ok = 0;
            }
            if (ok)
                total += ways;
        }
    }
    return total;
}
