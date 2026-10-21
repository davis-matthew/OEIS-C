/*
 * A250930  Number of (n+1)X(3+1) 0..3 arrays with no 2X2 subblock having a
 *          diagonal absolute difference less than its antidiagonal absolute
 *          difference.
 * https://oeis.org/A250930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   _init_adjacency
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250930
    Number of (n+1)×4 arrays with entries 0..3 such that
    every 2×2 subblock satisfies  
    |a[i][j]   - a[i+1][j+1]| ≥ |a[i][j+1] - a[i+1][j]|. 
    The array is computed by dynamic programming over rows.
    See comment above for details. */

#include <stdint.h>
#include "../utils/_init_adjacency.h"

seq_t a(idx_t n) {
    /* adjacency is initialized once */
    static int init = 0;
    static uint16_t neigh_cnt[256];
    static uint16_t neigh[256][256];
    if (!init) {
        for (int s = 0; s < 256; ++s) {
            uint8_t sp[4];
            for (int k = 0; k < 4; ++k) sp[k] = (s >> (k*2)) & 0x03;
            uint16_t cnt = 0;
            for (int t = 0; t < 256; ++t) {
                uint8_t tp[4];
                for (int k = 0; k < 4; ++k) tp[k] = (t >> (k*2)) & 0x03;
                int ok = 1;
                for (int j = 0; j < 3; ++j) {
                    int d1 = (int)sp[j] - (int)tp[j+1];
                    if (d1 < 0) d1 = -d1;
                    int d2 = (int)sp[j+1] - (int)tp[j];
                    if (d2 < 0) d2 = -d2;
                    if (d1 < d2) { ok = 0; break; }
                }
                if (ok) neigh[s][cnt++] = t;
            }
            neigh_cnt[s] = cnt;
        }
        init = 1;
    }

    /* DP arrays: one for previous and one for current step */
    static seq_t dp_prev[256];
    static seq_t dp_next[256];
    for (int s = 0; s < 256; ++s) {
        dp_prev[s] = 1;
    }

    for (idx_t step = 1; step <= n; ++step) {
        /* zero dp_next */
        for (int t = 0; t < 256; ++t) dp_next[t] = 0;
        /* transition */
        for (int s = 0; s < 256; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            for (uint16_t idx = 0; idx < neigh_cnt[s]; ++idx) {
                uint16_t t = neigh[s][idx];
                dp_next[t] += val;
            }
        }
        /* swap */
        for (int t = 0; t < 256; ++t) dp_prev[t] = dp_next[t];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int t = 0; t < 256; ++t) total += dp_prev[t];
    return total;
}
