/*
 * A251494  Number of (n+1)X(3+1) 0..3 arrays with every 2X2 subblock summing
 *          to 4 5 6 7 or 8.
 * https://oeis.org/A251494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A251494:
   Number of (n+1)×4 arrays over {0,1,2,3} where every 2×2 subblock sums to
   4,5,6,7 or 8.
   The routine counts all sequences of length n+1 of 4‑digit rows (each
   row encoded as a 2‑bit per digit value 0..3); two consecutive rows
   are allowed if all three overlapping 2×2 blocks meet the sum condition.
   A dynamic‑programming over the 256 possible rows yields the count. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---- build adjacency lists once ---- */
    static bool init_done = false;
    static uint8_t to[256][256];   /* to[i][k] = next row index from i */
    static uint8_t len[256];       /* number of outgoing neighbours of i */

    if (!init_done) {
        for (int i = 0; i < 256; ++i) {
            int d0 =  i        & 3;
            int d1 = (i >>  2) & 3;
            int d2 = (i >>  4) & 3;
            int d3 = (i >>  6) & 3;
            len[i] = 0;

            for (int j = 0; j < 256; ++j) {
                int e0 =  j        & 3;
                int e1 = (j >>  2) & 3;
                int e2 = (j >>  4) & 3;
                int e3 = (j >>  6) & 3;

                bool ok = true;
                int s0 = d0 + d1 + e0 + e1; if (s0 < 4 || s0 > 8) ok = false;
                int s1 = d1 + d2 + e1 + e2; if (ok && (s1 < 4 || s1 > 8)) ok = false;
                int s2 = d2 + d3 + e2 + e3; if (ok && (s2 < 4 || s2 > 8)) ok = false;

                if (ok) {
                    to[i][len[i]++] = (uint8_t)j;
                }
            }
        }
        init_done = true;
    }

    /* ---- DP over rows ---- */
    static seq_t cur[256];
    static seq_t nxt[256];

    /* start with a single row: one way to have any of the 256 rows */
    for (int i = 0; i < 256; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        /* zero next array */
        for (int i = 0; i < 256; ++i) nxt[i] = (seq_t)0;

        /* propagate counts */
        for (int i = 0; i < 256; ++i) {
            seq_t val = cur[i];
            if (val == (seq_t)0) continue;
            uint8_t *neigh = to[i];
            uint8_t   l    = len[i];
            for (uint8_t k = 0; k < l; ++k) {
                int j = neigh[k];
                nxt[j] += val;
            }
        }

        /* swap arrays */
        for (int i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    /* sum all final row counts to obtain the total number of arrays */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 256; ++i) result += cur[i];
    return result;
}
