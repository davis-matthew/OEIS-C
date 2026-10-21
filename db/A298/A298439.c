/*
 * A298439  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 binary arrays in which for each cell * the
 *            number of equal-valued king‑move adjacent cells is in
 *            {0,3,4,5,6}. * The upper‑left element must be 0. Computed by
 *            dynamic programming * over row patterns
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   rowIsValid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rowIsValid.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */
    seq_t total = 0;

    /* n == 1: only one row */
    if (n == 1) {
        for (int r0 = 0; r0 < 8; ++r0) {
            if ((r0 & 1) != 0) continue;   /* left‑most (col 0) must be 0 */
            if (rowIsValid(false, 0, r0, false, 0))
                ++total;
        }
        return total;
    }

    /* Dp arrays: dp[prev][cur] = number of ways up to current row cur
     * with previous row prev.  We need only 8×8 table. */
    seq_t dp[8][8] = { {0} };
    seq_t newdp[8][8] = { {0} };

    /* initialise with two first rows */
    for (int r0 = 0; r0 < 8; ++r0) {
        if ((r0 & 1) != 0) continue;   /* upper-left must be 0 */
        for (int r1 = 0; r1 < 8; ++r1) {
            if (rowIsValid(false, 0, r0, true, r1))
                dp[r0][r1] = dp[r0][r1] + 1;
        }
    }

    /* process rows 2 .. n-1 (indexing from 0) */
    for (idx_t step = 2; step < n; ++step) {
        /* clear newdp */
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                newdp[a][b] = 0;

        for (int prev = 0; prev < 8; ++prev) {
            for (int cur = 0; cur < 8; ++cur) {
                seq_t ways = dp[prev][cur];
                if (ways == 0) continue;
                for (int nxt = 0; nxt < 8; ++nxt) {
                    if (rowIsValid(true, prev, cur, true, nxt))
                        newdp[cur][nxt] = newdp[cur][nxt] + ways;
                }
            }
        }
        /* copy newdp into dp for next iteration */
        for (int a = 0; a < 8; ++a)
            for (int b = 0; b < 8; ++b)
                dp[a][b] = newdp[a][b];
    }

    /* last row must be valid with no next row */
    for (int prev = 0; prev < 8; ++prev) {
        for (int cur = 0; cur < 8; ++cur) {
            seq_t ways = dp[prev][cur];
            if (ways == 0) continue;
            if (rowIsValid(true, prev, cur, false, 0))
                total = total + ways;
        }
    }

    return total;
}
