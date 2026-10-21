/*
 * A299717  Number of nX4 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5, 6 or 7 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * Count of n×4 binary matrices where each cell is 0 or 1, the
 *            upper–left cell * is 0, and for every cell the number of
 *            neighboring cells (king moves) * that are equal to the cell’s
 *            own value is in {0,2,3,4,5,6,7}. * (i.e. the count may not be 1
 *            or, for interior cells, 8.) * * Recurrence: dynamic...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   valid_row_around
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/valid_row_around.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    if (n == 1) {
        /* Only one row, no neighbours below.  Count masks with
         * top‑left zero that satisfy the rule with no below row. */
        seq_t total = 0;
        for (int mask = 0; mask < 16; ++mask) {
            if ((mask & 1) == 0) {           /* top‑left zero */
                if (valid_row_around(0, mask, 0,
                                     0, /* no above */
                                     0) /* no below */
                    ) total += (seq_t)1;
            }
        }
        return total;
    }

    /* DP state: dp_prev[preprev][prev] = ways after processing rows
     * up to and including “prev”.  Initially preprev = dummy row of zeros,
     * prev = first row.                                                 */
    static seq_t dp_prev[16][16];
    static seq_t dp_curr[16][16];
    memset(dp_prev, 0, sizeof(dp_prev));

    for (int r1 = 0; r1 < 16; ++r1) {
        if ((r1 & 1) == 0)                /* top‑left zero for first row */
            dp_prev[0][r1] += (seq_t)1;
    }

    /* Process rows 2 … n.  On step r (2‑based index) we add row r,
     * validate row r‑1, whose above row exists only if r > 2.          */
    for (idx_t r = 2; r <= n; ++r) {
        memset(dp_curr, 0, sizeof(dp_curr));
        int has_above = (r > 2);          /* above row exists? */
        for (int pre = 0; pre < 16; ++pre) {
            for (int prev = 0; prev < 16; ++prev) {
                seq_t ways = dp_prev[pre][prev];
                if (!ways) continue;
                for (int cur = 0; cur < 16; ++cur) {
                    if (valid_row_around(pre, prev, cur, has_above, 1))
                        dp_curr[prev][cur] += ways;
                }
            }
        }
        memcpy(dp_prev, dp_curr, sizeof(dp_curr));
    }

    /* After all rows are processed, validate the last row (cur) with
     * no below row.  For n >= 2 the last row always has an above row.  */
    seq_t total = 0;
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            seq_t ways = dp_prev[prev][cur];
            if (!ways) continue;
            if (valid_row_around(prev, cur, 0, 1, 0))
                total += ways;
        }
    }
    return total;
}
