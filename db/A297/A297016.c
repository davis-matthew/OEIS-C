/*
 * A297016  Number of nX4 0..1 arrays with each 1 adjacent to 3, 4 or 6
 *          king-move neighboring 1s.
 * https://oeis.org/A297016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays such that every 1 has * 3, 4
 *            or 6 king–move neighbours (excluding itself). Computed by *
 *            dynamic programming over rows
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   mask_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mask_valid.h"

#define MAXMASK 16           /* 4 columns → 2^4 masks */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    /* Precompute all valid triples of rows. */
    static bool valid[MAXMASK][MAXMASK][MAXMASK];
    static bool init = false;
    if (!init) {
        for (int p = 0; p < MAXMASK; ++p)
            for (int c = 0; c < MAXMASK; ++c)
                for (int r = 0; r < MAXMASK; ++r)
                    valid[p][c][r] = mask_valid(p, c, r);
        init = true;
    }

    /* DP arrays: index = prev*MAXMASK + cur (0..255). */
    static seq_t dp[MAXMASK * MAXMASK];
    static seq_t ndp[MAXMASK * MAXMASK];
    for (int i = 0; i < MAXMASK * MAXMASK; ++i) dp[i] = 0;

    /* First row: previous row is all zeros. */
    for (int cur = 0; cur < MAXMASK; ++cur)
        dp[0 * MAXMASK + cur] = (seq_t)1;

    /* Add remaining rows. */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < MAXMASK * MAXMASK; ++i) ndp[i] = 0;
        for (int prev = 0; prev < MAXMASK; ++prev) {
            for (int cur = 0; cur < MAXMASK; ++cur) {
                seq_t val = dp[prev * MAXMASK + cur];
                if (!val) continue;
                for (int nxt = 0; nxt < MAXMASK; ++nxt) {
                    if (valid[prev][cur][nxt]) {
                        int idx = cur * MAXMASK + nxt;
                        ndp[idx] += val;
                    }
                }
            }
        }
        for (int i = 0; i < MAXMASK * MAXMASK; ++i) dp[i] = ndp[i];
    }

    /* Final check: last row with virtual zero row below. */
    seq_t result = 0;
    for (int prev = 0; prev < MAXMASK; ++prev) {
        for (int cur = 0; cur < MAXMASK; ++cur) {
            if (valid[prev][cur][0])
                result += dp[prev * MAXMASK + cur];
        }
    }

    return result;
}
