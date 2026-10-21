/*
 * A296153  Number of nX4 0..1 arrays with each 1 adjacent to 3 or 5 king-move
 *          neighboring 1s.
 * https://oeis.org/A296153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over rows of length 4. Each
 *            row is a 4‑bit pattern. The DP counts the ways to fill an n×4
 *            0/1 array where each 1 has exactly 3 or 5 king‑move neighboring
 *            1s
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* 3‑pattern validity table: valid[b][a][c] == 1 if a is valid when surrounded
   by rows b (above) and c (below). */
static uint8_t valid[16][16][16];
static bool valid_computed = false;

/* compute validity table for all triples of 4‑bit patterns */
static void compute_valid(void)
{
    if (valid_computed) return;
    for (int b = 0; b < 16; ++b)
    {
        for (int a = 0; a < 16; ++a)
        {
            for (int c = 0; c < 16; ++c)
            {
                bool ok = true;
                for (int i = 0; i < 4 && ok; ++i)
                {
                    if ((a & (1 << i)) == 0) continue;   /* only 1s need checking */

                    int cnt = 0;
                    /* same row left/right */
                    if (i > 0 && (a & (1 << (i-1)))) ++cnt;
                    if (i < 3 && (a & (1 << (i+1)))) ++cnt;
                    /* above row */
                    if (i > 0 && (b & (1 << (i-1)))) ++cnt;
                    if (b & (1 << i))                 ++cnt;
                    if (i < 3 && (b & (1 << (i+1))))  ++cnt;
                    /* below row */
                    if (i > 0 && (c & (1 << (i-1)))) ++cnt;
                    if (c & (1 << i))                 ++cnt;
                    if (i < 3 && (c & (1 << (i+1))))  ++cnt;

                    if (!(cnt == 3 || cnt == 5)) ok = false;
                }
                valid[b][a][c] = ok ? 1 : 0;
            }
        }
    }
    valid_computed = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for indices below the offset */

    compute_valid();

    /* DP tables: dp[b][a] stores number of ways for the already processed
       rows whose last two rows are patterns b and a. */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];

    /* zero dp */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    /* initialise with first row only; above sentinel = 0 */
    for (int p = 0; p < 16; ++p)
        dp[0][p] = (seq_t)1;

    /* process remaining rows 2..n */
    for (idx_t r = 2; r <= n; ++r)
    {
        /* clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (int b = 0; b < 16; ++b)
        {
            for (int a = 0; a < 16; ++a)
            {
                seq_t ways = dp[b][a];
                if (ways == 0) continue;
                for (int c = 0; c < 16; ++c)
                {
                    if (valid[b][a][c])
                        ndp[a][c] += ways;
                }
            }
        }

        /* copy ndp -> dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final check: below sentinel = 0 */
    seq_t result = 0;
    for (int b = 0; b < 16; ++b)
    {
        for (int a = 0; a < 16; ++a)
        {
            if (dp[b][a] != 0 && valid[b][a][0])
                result += dp[b][a];
        }
    }
    return result;
}
