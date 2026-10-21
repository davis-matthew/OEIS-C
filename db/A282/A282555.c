/*
 * A282555  Number of nX3 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors, with the exception of exactly one element.
 * https://oeis.org/A282555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   violation_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A282555 by dynamic programming over an n×3 binary array.
   The DP keeps the last two rows and whether a violating 1‑cell has already appeared.
   Each transition adds a new row, checks the constraint for the middle row,
   and updates the count.  Finally the last row is checked with a virtual zero row
   below. */

#include <stdint.h>
#include "../utils/violation_count.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* dp[prev][prevprev][bad] */
    seq_t dp[8][8][2] = {0};
    seq_t nextdp[8][8][2] = {0};

    /* initial state after first row */
    for (uint8_t cur = 0; cur < 8; ++cur)
        dp[cur][0][0] += 1;

    for (idx_t row = 2; row <= n; ++row)
    {
        /* zero nextdp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                nextdp[i][j][0] = nextdp[i][j][1] = 0;

        for (uint8_t up = 0; up < 8; ++up)
        {
            for (uint8_t cur = 0; cur < 8; ++cur)
            {
                for (int bad = 0; bad < 2; ++bad)
                {
                    seq_t val = dp[cur][up][bad];
                    if (val == 0)
                        continue;
                    for (uint8_t newrow = 0; newrow < 8; ++newrow)
                    {
                        int v = violation_count(up, cur, newrow);
                        if (v > 1)
                            continue;                /* more than one bad cell in this row */
                        if (bad == 1 && v >= 1)
                            continue;                /* already had a bad cell */
                        int newBad = bad + v;
                        nextdp[newrow][cur][newBad] += val;
                    }
                }
            }
        }

        /* copy nextdp into dp */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
            {
                dp[i][j][0] = nextdp[i][j][0];
                dp[i][j][1] = nextdp[i][j][1];
            }
    }

    /* final evaluation of the last row with a virtual zero row below */
    seq_t result = 0;
    uint8_t zeroRow = 0;
    for (uint8_t up = 0; up < 8; ++up)
    {
        for (uint8_t cur = 0; cur < 8; ++cur)
        {
            for (int bad = 0; bad < 2; ++bad)
            {
                seq_t val = dp[cur][up][bad];
                if (val == 0)
                    continue;
                int v = violation_count(up, cur, zeroRow);
                if (v > 1)
                    continue;
                if (bad == 1 && v >= 1)
                    continue;
                int newBad = bad + v;
                if (newBad == 1)
                    result += val;
            }
        }
    }

    return result;
}
