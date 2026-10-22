/*
 * A303103  Number of 3Xn 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   valid_col_mid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A303103:
    Number of 3×n 0..1 arrays with every element having 1, 2, 3, 4 or 6 adjacent
    cells (horizontally or diagonally – vertical adjacency is NOT counted) that
    are equal to the element itself, with upper‑left element zero.
    Computation is done by a transfer‑matrix dynamic programme over columns. */

#include <stdbool.h>
#include "../utils/valid_col_mid.h"

#define MAX_STATES 8   /* 3‑bit column patterns: 0..7 */

/* ------------------------------------------------------------------------- */
/* Main routine: count the arrays for a given n (n>=1).                       */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Handle n == 1 separately: only the first column exists.            */
    if (n == 1) {
        seq_t res = 0;
        for (uint8_t p0 = 0; p0 < MAX_STATES; ++p0) {
            if (p0 & 1) continue;                     /* upper‑left must be 0 */
            if (valid_col_mid(p0, 0, 0, false, false))
                res += 1;
        }
        return res;
    }

    seq_t result = 0;

    /* Temporary DP tables.  dp[prevPrev][prev] holds the number of ways
       to reach the current position with the last two columns equal to
       prevPrev and prev.  Values are small enough for seq_t.              */
    seq_t dp[8][8] = {0};
    seq_t ndp[8][8] = {0};

    /* For every admissible first column pattern p0 (upper‑left zero)       */
    for (uint8_t p0 = 0; p0 < MAX_STATES; ++p0) {
        if (p0 & 1) continue;                     /* upper‑left must be 0 */

        /* For every possible second column pattern p1 (no restriction)    */
        for (uint8_t p1 = 0; p1 < MAX_STATES; ++p1) {

            /* Validate the first column: left boundary is absent,
               right neighbour is p1.                                      */
            if (!valid_col_mid(p0, 0, p1, false, true))
                continue;

            /* Initialise DP with this pair as the first two columns.      */
            for (int i = 0; i < MAX_STATES; ++i)
                for (int j = 0; j < MAX_STATES; ++j)
                    dp[i][j] = 0;
            dp[p0][p1] = 1;

            /* Process remaining columns 2 .. n-1 (total n-2 further)      */
            for (idx_t col = 2; col < n; ++col) {
                /* New column index is 'col', pattern to try is 'cur'.     */
                for (int i = 0; i < MAX_STATES; ++i)
                    for (int j = 0; j < MAX_STATES; ++j)
                        ndp[i][j] = 0;

                for (int prevPrev = 0; prevPrev < MAX_STATES; ++prevPrev) {
                    for (int prev = 0; prev < MAX_STATES; ++prev) {
                        seq_t ways = dp[prevPrev][prev];
                        if (!ways) continue;
                        for (uint8_t cur = 0; cur < MAX_STATES; ++cur) {
                            if (!valid_col_mid(prev, prevPrev, cur,
                                               true,  true))
                                continue;
                            ndp[prev][cur] += ways;
                        }
                    }
                }

                /* Advance to next column. */
                for (int i = 0; i < MAX_STATES; ++i)
                    for (int j = 0; j < MAX_STATES; ++j)
                        dp[i][j] = ndp[i][j];
            }

            /* After all columns processed, validate the last column (prev)
               with right boundary absent.                                */
            for (int prevPrev = 0; prevPrev < MAX_STATES; ++prevPrev) {
                for (int prev = 0; prev < MAX_STATES; ++prev) {
                    seq_t ways = dp[prevPrev][prev];
                    if (!ways) continue;
                    if (valid_col_mid(prev, prevPrev, 0,
                                       true, false))
                        result += ways;
                }
            }
        }
    }

    return result;
}
