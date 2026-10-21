/*
 * A251256  Number of (n+1)X(6+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge.
 * https://oeis.org/A251256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×7 binary arrays in which every 2×2 *
 *            subblock contains either a single ‘1’ or exactly two ‘1’s that *
 *            lie on the same edge. This sequence is OEIS A251256. * We
 *            compute it by dynamic programming over the 128 possible 7‑bit *
 *            rows, using the compatibility relation between con...
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   is_compatible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_compatible.h"

/* Precomputed adjacency */
static int comp_list[128][128];
static int comp_cnt[128];
static bool adjacency_inited = false;
static void init_adjacency(void)
{
    if (adjacency_inited) return;
    for (int p = 0; p < 128; ++p) {
        int idx = 0;
        for (int q = 0; q < 128; ++q) {
            if (is_compatible((uint8_t)p, (uint8_t)q))
                comp_list[p][idx++] = q;
        }
        comp_cnt[p] = idx;
    }
    adjacency_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_adjacency();

    /* Number of rows = n + 1 */
    idx_t rows = n + 1;
    if (rows == 0) return 0;            /* should not happen (n≥1) */

    seq_t dp_cur[128];
    seq_t dp_next[128];
    seq_t zero = 0;

    /* initial distribution: any single row is possible */
    for (int i = 0; i < 128; ++i) dp_cur[i] = 1;

    /* iterate over the remaining rows */
    for (idx_t step = 0; step < rows - 1; ++step) {
        /* zero dp_next */
        for (int i = 0; i < 128; ++i) dp_next[i] = zero;

        for (int p = 0; p < 128; ++p) {
            if (dp_cur[p] == 0) continue;
            seq_t val = dp_cur[p];
            int sz = comp_cnt[p];
            for (int k = 0; k < sz; ++k) {
                int q = comp_list[p][k];
                dp_next[q] = dp_next[q] + val;
            }
        }
        /* prepare for next step */
        for (int i = 0; i < 128; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all ending rows to get the total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < 128; ++i) total = total + dp_cur[i];
    return total;
}
