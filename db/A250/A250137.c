/*
 * A250137  Number of length n+2 0..5 arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 31, u128 n <= 71,
 *            big any n
 * Helpers:   state_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250137: a(n) – number of length n+2 arrays over {0..5} with
 * nondecreasing medians of every three consecutive terms */

#include <stddef.h>
#include "../utils/state_index.h"

/* Compute the median of three 0..5 values */
static inline uint8_t median3(uint8_t a, uint8_t b, uint8_t c) {
    uint8_t min = a;
    if (b < min) min = b;
    if (c < min) min = c;
    uint8_t max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return (uint8_t)(a + b + c - min - max);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Two DP tables of size 6*6*6 = 216 */
    seq_t dp0[216];
    seq_t dp1[216];
    seq_t *cur = dp0;
    seq_t *nxt = dp1;

    /* initialize all states to zero */
    for (int i = 0; i < 216; ++i) cur[i] = (seq_t)0;

    /* initial triples: a0,a1,a2 → state (x=a1, y=a2, m=median(a0,a1,a2)) */
    for (int a0 = 0; a0 < 6; ++a0) {
        for (int a1 = 0; a1 < 6; ++a1) {
            for (int a2 = 0; a2 < 6; ++a2) {
                uint8_t m = median3((uint8_t)a0, (uint8_t)a1, (uint8_t)a2);
                int idx = state_index(a1, a2, m);
                cur[idx] += (seq_t)1;
            }
        }
    }

    /* number of further elements to add after the initial 3 */
    idx_t steps = n - 1;

    for (idx_t step = 0; step < steps; ++step) {
        /* zero next table */
        for (int i = 0; i < 216; ++i) nxt[i] = (seq_t)0;

        /* transitions */
        for (int x = 0; x < 6; ++x) {
            for (int y = 0; y < 6; ++y) {
                for (int m = 0; m < 6; ++m) {
                    int idx_cur = state_index(x, y, m);
                    seq_t val = cur[idx_cur];
                    if (val == (seq_t)0) continue;
                    for (int z = 0; z < 6; ++z) {
                        uint8_t new_m = median3((uint8_t)x, (uint8_t)y, (uint8_t)z);
                        if ((uint8_t)m <= new_m) {
                            int idx_nxt = state_index(y, z, new_m);
                            nxt[idx_nxt] += val;
                        }
                    }
                }
            }
        }
        /* swap tables */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum all states */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 216; ++i) {
        result += cur[i];
    }

    return result;
}
