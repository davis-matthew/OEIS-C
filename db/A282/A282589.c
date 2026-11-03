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
 * Validated: standard, pass, 34 terms through n = 176 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Precompute the number of violations among the 1‑cells in the middle row
 * of a triple (prev, cur, next) of 4‑bit patterns.  A violation occurs when
 * a 1 has more than two 1‑neighbours.  Data fits in a uint8_t.  The table
 * is built once on the first call. */
static const uint8_t *calc_violations(void) {
    static uint8_t viol[16][16][16];
    static bool built = false;
    if (built) return (const uint8_t *)viol;
    for (uint8_t prev = 0; prev < 16; ++prev) {
        for (uint8_t cur  = 0; cur  < 16; ++cur) {
            for (uint8_t next = 0; next < 16; ++next) {
                uint8_t v = 0;
                for (int c = 0; c < 4; ++c) {
                    if (!(cur & (1 << c))) continue;          /* cell is 0 → no test */
                    int cnt = 0;
                    /* loop over the 8 king offsets */
                    for (int dr = -1; dr <= 1; ++dr) {
                        for (int dc = -1; dc <= 1; ++dc) {
                            if (dr == 0 && dc == 0) continue;   /* skip self */
                            int col = c + dc;
                            if (col < 0 || col > 3) continue;  /* out of bounds */
                            uint8_t pat;
                            if (dr == -1) pat = prev;
                            else if (dr == 0) pat = cur;
                            else pat = next;
                            if (pat & (1 << col)) ++cnt;
                        }
                    }
                    if (cnt > 2) ++v;
                }
                viol[prev][cur][next] = v;
            }
        }
    }
    built = true;
    return (const uint8_t *)viol;
}

/* Helper: convert 4‑bit pattern to its bit mask */
static inline uint16_t mask(uint8_t pat) { return (uint16_t)pat; }

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* sequence offset is 1 */

    /* Map a 3‑dimensional indices to a flat array for speed. */
    const uint8_t *viol = calc_violations();    /* 16x16x16 table (flattened) */
    /* The index for pattern (prev,cur) is prev*16 + cur */
    static seq_t dp[256][3];
    static seq_t next[256][3];
    mem_set(dp, 0, sizeof(dp));

    /* initial state – we have a sentinel row of zeros (index 0) above
     * the first actual row.  No violations yet. */
    for (int cur = 0; cur < 16; ++cur) dp[0 * 16 + cur][0] = (seq_t)1;

    /* Process rows 2 … n.  At step i we add row i and finalize row i−1. */
    for (idx_t i = 2; i <= n; ++i) {
        mem_set(next, 0, sizeof(next));
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
        mem_copy(dp, next, sizeof(dp));
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
