/*
 * A253692  Number of (n+1)X(2+1) 0..1 arrays with every 2X2 subblock ne-sw
 *          antidiagonal difference unequal to its neighbors horizontally and
 *          nw+se diagonal sum unequal to its neighbors vertically.
 * https://oeis.org/A253692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A253692: Number of (n+1)×3 binary arrays satisfying the
   horizontal “NE–SW” difference inequality and the vertical
   “NW+SE” sum inequality.
   The computation is performed by a simple DP over a graph of
   64 edges: edges are pairs of rows (8 possible rows) that satisfy
   the horizontal condition.  Each edge is labeled by the two
   sums (s0,s1).  A valid path of length n represents n+1 rows,
   and the vertical condition enforces that consecutive edges
   have different labels in each coordinate.  Complexity:
   O(n·E²) with E=64, easily fast for n≈10⁴. */

#include <stddef.h>

typedef struct {
    unsigned src, dst;      /* source and destination row (0..7)       */
    unsigned char s0, s1;   /* sums a(0)+b(1) and a(1)+b(2) for the pair */
} Edge;

seq_t a(idx_t n) {
    /* Handle the trivial (invalid) case */
    if (n == 0) return 0;

    static int init = 0;          /* one‑time initialization flag            */
    static Edge edges[64];
    static size_t edge_cnt = 0;
    static size_t next_idx[64][64];
    static size_t next_count[64];

    if (!init) {
        /* Build all valid edges */
        for (unsigned src = 0; src < 8; ++src) {
            unsigned r0 = src & 1;
            unsigned r1 = (src >> 1) & 1;
            unsigned r2 = (src >> 2) & 1;
            for (unsigned dst = 0; dst < 8; ++dst) {
                unsigned t0 = dst & 1;
                unsigned t1 = (dst >> 1) & 1;
                unsigned t2 = (dst >> 2) & 1;
                int diff0 = (int)r1 - (int)t0;
                int diff1 = (int)r2 - (int)t1;
                if (diff0 != diff1) {                     /* horizontal rule */
                    edges[edge_cnt].src = src;
                    edges[edge_cnt].dst = dst;
                    edges[edge_cnt].s0  = (unsigned char)(r0 + t1);
                    edges[edge_cnt].s1  = (unsigned char)(r1 + t2);
                    ++edge_cnt;
                }
            }
        }

        /* Pre‑compute admissible next edges for each edge */
        for (size_t i = 0; i < edge_cnt; ++i) {
            next_count[i] = 0;
            for (size_t j = 0; j < edge_cnt; ++j) {
                if (edges[i].dst == edges[j].src &&
                    edges[i].s0  != edges[j].s0 &&
                    edges[i].s1  != edges[j].s1) {
                    next_idx[i][next_count[i]++] = j;
                }
            }
        }
        init = 1;
    }

    /* Dynamic programming over the edge graph */
    seq_t dp_prev[64] = {0};
    seq_t dp_next[64] = {0};

    /* Length‑1 paths: one edge each */
    for (size_t i = 0; i < edge_cnt; ++i) dp_prev[i] = 1;

    /* Extend paths upto length n */
    for (idx_t step = 2; step <= n; ++step) {
        for (size_t i = 0; i < edge_cnt; ++i) dp_next[i] = 0;
        for (size_t i = 0; i < edge_cnt; ++i) {
            seq_t val = dp_prev[i];
            if (val == 0) continue;
            for (size_t k = 0; k < next_count[i]; ++k) {
                size_t j = next_idx[i][k];
                dp_next[j] = dp_next[j] + val;
            }
        }
        for (size_t i = 0; i < edge_cnt; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum over paths of length n */
    seq_t result = 0;
    for (size_t i = 0; i < edge_cnt; ++i) result += dp_prev[i];
    return result;
}
