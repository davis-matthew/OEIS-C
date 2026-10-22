/*
 * A305171  Number of nX4 0..1 arrays with every element unequal to 2, 3, 4, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS A305171:
    Number of n×4 0..1 arrays with every element unequal to
    2, 3, 4, 6 or 7 king-move adjacent elements, with upper left element zero.
    The algorithm uses a row-by-row dynamic programming over the 4 columns.
*/
#include <stdint.h>
#include <stdbool.h>

static inline bool valid_row(uint8_t top, uint8_t mid, uint8_t bottom,
                             bool topExists, bool bottomExists)
{
    /* each row is 4 bits, columns 0..3 */
    for (int c = 0; c < 4; ++c)
    {
        uint8_t val = (mid >> c) & 1;
        int cnt = 0;

        /* left and right neighbours in the same row */
        if (c > 0   && (((mid >> (c-1)) & 1) != val)) ++cnt;
        if (c < 3   && (((mid >> (c+1)) & 1) != val)) ++cnt;

        /* neighbours above */
        if (topExists)
        {
            if (c > 0   && (((top >> (c-1)) & 1) != val)) ++cnt;
            if (c < 3   && (((top >> (c+1)) & 1) != val)) ++cnt;
            if (((top >> c) & 1) != val) ++cnt;
        }

        /* neighbours below */
        if (bottomExists)
        {
            if (c > 0   && (((bottom >> (c-1)) & 1) != val)) ++cnt;
            if (c < 3   && (((bottom >> (c+1)) & 1) != val)) ++cnt;
            if (((bottom >> c) & 1) != val) ++cnt;
        }

        /* allowed numbers of unequal neighbours: 2,3,4,6,7 */
        if (!((cnt==2)||(cnt==3)||(cnt==4)||(cnt==6)||(cnt==7)))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1, not used */
    if (n == 1)                            /* single row case */
    {
        seq_t res = 0;
        for (uint8_t r = 0; r < 16; ++r)
        {
            if (r & 1) continue;           /* upper left must be 0 */
            if (valid_row(0, r, 0, false, false))
                res += 1;
        }
        return res;
    }

    /* DP arrays: dp[prevprev][prev] = number of ways for processed rows */
    seq_t dp[16][16]   = {{0}};
    seq_t ndp[16][16]  = {{0}};
    /* initialise with the first two rows */
    for (uint8_t r0 = 0; r0 < 16; ++r0)
    {
        if (r0 & 1) continue;               /* upper left 0 */
        for (uint8_t r1 = 0; r1 < 16; ++r1)
        {
            if (valid_row(0, r0, r1, false, true))
                dp[r0][r1] = 1;
        }
    }

    /* process rows 2 .. n-1 */
    for (idx_t row = 2; row < n; ++row)
    {
        /* clear ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (int pprev = 0; pprev < 16; ++pprev)
        {
            for (int prev = 0; prev < 16; ++prev)
            {
                seq_t count = dp[pprev][prev];
                if (!count) continue;
                for (int cur = 0; cur < 16; ++cur)
                {
                    if (valid_row(pprev, prev, cur, true, true))
                        ndp[prev][cur] += count;
                }
            }
        }

        /* swap dp and ndp */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final row validation (no row below) */
    seq_t ans = 0;
    for (int prevprev = 0; prevprev < 16; ++prevprev)
    {
        for (int prev = 0; prev < 16; ++prev)
        {
            seq_t count = dp[prevprev][prev];
            if (!count) continue;
            if (valid_row(prevprev, prev, 0, true, false))
                ans += count;
        }
    }
    return ans;
}
