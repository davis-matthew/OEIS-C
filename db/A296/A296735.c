/*
 * A296735  Number of nX4 0..1 arrays with each 1 adjacent to 1, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary n×4 arrays such that every cell with
 *            value 1 has 1, 2 or 4 king-move neighboring 1s. The
 *            implementation uses a DP over three consecutive rows: for each
 *            pair (previous‑previous, previous) the DP stores how many ways
 *            the matrix can be built up to that point. When a new row i...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   count_single_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_single_row.h"

static bool valid_tri[16][16][16];  /* pre‑computed validity of (pp,p,c) */

/* initialise the table of valid triples */
static void init_valid(void)
{
    for (int pp = 0; pp < 16; ++pp)
        for (int p  = 0; p  < 16; ++p)
            for (int c  = 0; c  < 16; ++c)
            {
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j)
                {
                    if (!(p & (1 << j))) continue;   /* only 1-cells matter */

                    int cnt = 0;
                    /* left/right in same row */
                    if (j > 0   && (p & (1 << (j-1)))) ++cnt;
                    if (j < 3   && (p & (1 << (j+1)))) ++cnt;
                    /* up-left/up/up-right */
                    if (j > 0   && (pp & (1 << (j-1)))) ++cnt;
                    if (pp & (1 << j)) ++cnt;
                    if (j < 3   && (pp & (1 << (j+1)))) ++cnt;
                    /* down-left/down/down-right */
                    if (j > 0   && (c & (1 << (j-1)))) ++cnt;
                    if (c & (1 << j)) ++cnt;
                    if (j < 3   && (c & (1 << (j+1)))) ++cnt;

                    if (cnt != 1 && cnt != 2 && cnt != 4)
                        ok = false;
                }
                valid_tri[pp][p][c] = ok;
            }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset, undefined, return 0 */
    if (n == 1) return count_single_row();

    /* ensure valid_tri is initialised once */
    static bool initialized = false;
    if (!initialized) { init_valid(); initialized = true; }

    /* DP arrays */
    static seq_t dp[16][16];
    static seq_t ndp[16][16];
    /* initialize dp for first row with implicit empty previous row */
    for (int pp = 0; pp < 16; ++pp)
        for (int p = 0; p < 16; ++p)
            dp[pp][p] = 0;
    for (int m = 0; m < 16; ++m) dp[0][m] = 1;

    /* iterate adding rows 2..n */
    for (idx_t row = 2; row <= n; ++row)
    {
        for (int pp = 0; pp < 16; ++pp)
            for (int p = 0; p < 16; ++p)
                ndp[pp][p] = 0;

        for (int pp = 0; pp < 16; ++pp)
        {
            for (int p = 0; p < 16; ++p)
            {
                seq_t ways = dp[pp][p];
                if (!ways) continue;
                for (int c = 0; c < 16; ++c)
                {
                    if (!valid_tri[pp][p][c]) continue;
                    ndp[p][c] += ways;
                }
            }
        }
        /* copy ndp to dp */
        for (int pp = 0; pp < 16; ++pp)
            for (int p = 0; p < 16; ++p)
                dp[pp][p] = ndp[pp][p];
    }

    /* final validation with imaginary empty row after last row */
    seq_t ans = 0;
    for (int pp = 0; pp < 16; ++pp)
        for (int p = 0; p < 16; ++p)
            if (valid_tri[pp][p][0])
                ans += dp[pp][p];

    return ans;
}
