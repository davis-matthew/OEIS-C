/*
 * A282589  Number of nX4 0..1 arrays with no 1 equal to more than two of its
 *          king-move neighbors, with the exception of exactly two elements.
 * https://oeis.org/A282589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays in which exactly two 1‑cells
 *            * have more than two king‑move neighbours (neighbours are the 8
 *            surrounding * cells). Computed via a linear–time dynamic
 *            programming over the rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   calc_violations, mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/calc_violations.h"
#include "../utils/mask.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* sequence offset is 1 */

    /* Map a 3‑dimensional indices to a flat array for speed. */
    const uint8_t *viol = calc_violations();    /* 16x16x16 table (flattened) */
    /* The index for pattern (prev,cur) is prev*16 + cur */
    static seq_t dp[256][3];
    static seq_t next[256][3];
    memset(dp, 0, sizeof(dp));

    /* initial state – we have a sentinel row of zeros (index 0) above
     * the first actual row.  No violations yet. */
    for (int cur = 0; cur < 16; ++cur) dp[0 * 16 + cur][0] = (seq_t)1;

    /* Process rows 2 … n.  At step i we add row i and finalize row i−1. */
    for (idx_t i = 2; i <= n; ++i) {
        memset(next, 0, sizeof(next));
        for (int idx = 0; idx < 256; ++idx) {
            int prev = idx >> 4;            /* high 4 bits */
            int cur  = idx & 15;            /* low  4 bits */
            for (int k = 0; k <= 2; ++k) {
                seq_t val = dp[idx][k];
                if (!val) continue;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    uint8_t v = viol[(prev << 8) | (cur << 4) | nxt];
                    uint8_t newk = k + v;
                    if (newk > 2) continue;
                    int newIdx = cur * 16 + nxt;
                    next[newIdx][newk] += val;
                }
            }
        }
        /* swap dp and next */
        memcpy(dp, next, sizeof(dp));
    }

    /* Final step – finalize the last row using a sentinel row of zeros below. */
    seq_t result = 0;
    for (int idx = 0; idx < 256; ++idx) {
        int prev = idx >> 4;
        int cur  = idx & 15;
        for (int k = 0; k <= 2; ++k) {
            seq_t val = dp[idx][k];
            if (!val) continue;
            uint8_t v = viol[(prev << 8) | (cur << 4) | 0];  /* next=0 sentinel */
            if (k + v == 2) result += val;
        }
    }
    return result;
}
