/*
 * A283126  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A283126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A283126: Number of n×4 0..1 arrays with no 1 having more than two horizontal/vertical neighbours.
 *
 * Computation by transfer‑matrix DP over rows.
 * We keep the last two row patterns (as 4‑bit numbers) and, for every transition
 * add a new row pattern only if all 1‑cells in the middle row meet the
 * neighbour limit.  After the final row we check its cells with a missing
 * lower neighbour (treated as 0).
 *
 * This implementation uses an O(n) algorithm with a 16×16×16 state table,
 * which is precomputed once.  It works for any seq_t/idx_t sizes supported
 * by the build system.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computation tables, built once on first call */
    static int computed = 0;
    static unsigned char h[16][4];          /* horizontal neighbours in a row */
    static uint8_t valid[16][16][16];       /* transition feasibility */
    static uint8_t final_ok[16][16];        /* final row feasibility */

    if (!computed)
    {
        /* horizontal neighbour count for each pattern */
        for (int p = 0; p < 16; ++p)
        {
            for (int j = 0; j < 4; ++j)
            {
                unsigned char left  = (j > 0)     ? ((p >> (j-1)) & 1) : 0;
                unsigned char right = (j < 3)     ? ((p >> (j+1)) & 1) : 0;
                h[p][j] = left + right;
            }
        }

        /* transition validity */
        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
                for (int next = 0; next < 16; ++next)
                {
                    uint8_t ok = 1;
                    for (int j = 0; j < 4; ++j)
                        if (curr & (1 << j))
                        {
                            int neigh = h[curr][j] + ((prev >> j) & 1) + ((next >> j) & 1);
                            if (neigh > 2)
                            {
                                ok = 0;
                                break;
                            }
                        }
                    valid[prev][curr][next] = ok;
                }

        /* final row check (no below neighbour) */
        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
            {
                uint8_t ok = 1;
                for (int j = 0; j < 4; ++j)
                    if (curr & (1 << j))
                    {
                        int neigh = h[curr][j] + ((prev >> j) & 1);
                        if (neigh > 2)
                        {
                            ok = 0;
                            break;
                        }
                    }
                final_ok[prev][curr] = ok;
            }

        computed = 1;
    }

    /* DP tables */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* initialise dp for first row (row 1) */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;
    for (int curr = 0; curr < 16; ++curr)
        dp[0][curr] = 1;          /* previous row is a dummy 0 */
    
    /* process rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        for (int prev = 0; prev < 16; ++prev)
            for (int curr = 0; curr < 16; ++curr)
            {
                seq_t v = dp[prev][curr];
                if (v == 0) continue;
                for (int next = 0; next < 16; ++next)
                    if (valid[prev][curr][next])
                        ndp[curr][next] += v;
            }

        /* copy ndp to dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final answer: check last row */
    seq_t ans = 0;
    for (int prev = 0; prev < 16; ++prev)
        for (int curr = 0; curr < 16; ++curr)
        {
            seq_t v = dp[prev][curr];
            if (v == 0) continue;
            if (final_ok[prev][curr])
                ans += v;
        }

    return ans;
}
