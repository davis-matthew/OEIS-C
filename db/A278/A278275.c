/*
 * A278275  Number of n X 3 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   satisfies_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count n×3 binary arrays (top‑left cell fixed to 0) where each
   cell has at least one equal and one unequal neighbour among the six
   offsets (-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,0).  The state consists
   of two consecutive rows; transitions use the next row to check the
   constraints for the middle row.  The DP has 8×8 (=64) states for the
   pair of rows, which is enough to handle n ≤ 10 000 in well under
   2 s.  The algorithm is exact for any integer type that supports the
   required arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/satisfies_row.h"

seq_t a(idx_t n)
{
    /* Return 0 for the trivial case n=1. */
    if (n == 1) return 0;

    /* Precompute the small tables only once. */
    static bool vp[8][8];
    static bool vm[8][8][8];
    static bool vl[8][8];
    static bool tables_ready = false;

    if (!tables_ready) {
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
                for (int down = 0; down < 8; ++down) {
                    vp[up][down] = satisfies_row(0, up, down, false, true);
                    vm[up][cur][down] = satisfies_row(up, cur, down, true, true);
                }
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur)
                vl[up][cur] = satisfies_row(up, cur, 0, true, false);
        tables_ready = true;
    }

    /* DP arrays: dp[prevprev][prev] holds the number of arrays up to
       the current row with that last pair of rows. */
    seq_t dp[8][8] = {0};
    seq_t newdp[8][8];

    /* Initial step: rows 0 and 1.  Row 0 must start with a 0 in the
       leftmost column. */
    for (int row0 = 0; row0 < 8; ++row0) {
        if ((row0 & 1) == 0)      /* leftmost cell zero */
            for (int row1 = 0; row1 < 8; ++row1)
                if (vp[row0][row1])
                    dp[row0][row1] = dp[row0][row1] + 1;
    }

    /* Build further rows.  For n≥3 we need to add rows 2…n‑1. */
    for (idx_t step = 2; step < n; ++step) {
        /* Reset newdp. */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                newdp[i][j] = 0;
        for (int up = 0; up < 8; ++up)
            for (int cur = 0; cur < 8; ++cur) {
                seq_t val = dp[up][cur];
                if (!val) continue;
                for (int down = 0; down < 8; ++down)
                    if (vm[up][cur][down])
                        newdp[cur][down] = newdp[cur][down] + val;
            }
        /* Copy back to dp. */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over valid final rows (no down neighbour). */
    seq_t result = 0;
    for (int up = 0; up < 8; ++up)
        for (int cur = 0; cur < 8; ++cur)
            if (dp[up][cur] && vl[up][cur])
                result = result + dp[up][cur];

    return result;
}
