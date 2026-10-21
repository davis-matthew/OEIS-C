/*
 * A296669  Number of nX3 0..1 arrays with each 1 adjacent to 0, 2 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   test_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296669: Number of n×3 0..1 arrays where each 1 has 0, 2 or 3 king‑neighbouring 1s. */

/* This implementation uses dynamic programming on 3‑cell rows.  The lookup table
 * `comp[u][p][d]` stores whether a middle row `p` is valid when its upper row
 * is `u` and its lower row is `d`.  The table is filled once on first call.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/test_row.h"

seq_t a(idx_t n) {
    enum { P = 8 };                     /* 3‑bit patterns: 0 .. 7 */
    static bool comp[P][P][P];
    static bool comp_inited = false;
    if (!comp_inited) {
        for (int u = 0; u < P; ++u)
            for (int p = 0; p < P; ++p)
                for (int d = 0; d < P; ++d)
                    comp[u][p][d] = test_row((uint8_t)p, (uint8_t)u, (uint8_t)d);
        comp_inited = true;
    }

    /* dp[u][p] = number of ways to fill processed rows with upper row pattern u
     * and current row pattern p (p is already valid with its upper neighbour). */
    seq_t dp[P][P] = {0};
    seq_t ndp[P][P];

    /* first row, no upper neighbours */
    for (int p = 0; p < P; ++p)
        dp[0][p] = 1;

    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < P; ++i)
            for (int j = 0; j < P; ++j)
                ndp[i][j] = 0;
        for (int u = 0; u < P; ++u)
            for (int p = 0; p < P; ++p) {
                seq_t val = dp[u][p];
                if (!val)
                    continue;
                for (int d = 0; d < P; ++d)
                    if (comp[u][p][d])
                        ndp[p][d] = ndp[p][d] + val;
            }
        for (int i = 0; i < P; ++i)
            for (int j = 0; j < P; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* last row must be valid with no lower neighbours */
    seq_t ans = 0;
    for (int u = 0; u < P; ++u)
        for (int p = 0; p < P; ++p)
            if (comp[u][p][0])
                ans = ans + dp[u][p];

    return ans;
}
