/*
 * A250971  Number of (n+1)X(6+1) 0..1 arrays with no 2X2 subblock having the
 *          sum of its diagonal elements less than the absolute difference of
 *          its antidiagonal elements.
 * https://oeis.org/A250971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×7 binary arrays that avoid any 2×2
 *            subblock with a diagonal sum less than the absolute difference
 *            of its antidiagonal elements. Equivalently, for each adjacent
 *            pair of rows s and t (encoded as 7‑bit masks) the transition is
 *            allowed iff for every column c = 0..5 not (a...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   allowed_transition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/allowed_transition.h"

#define S 128

/* adjacency lists: for each state s we store the list of allowed next states */
static unsigned char adj_next[S][S];
static unsigned char adj_count[S];
static bool adj_built = false;

/* build the adjacency lists once */
static void build_adjacency(void)
{
    for (idx_t s = 0; s < S; ++s) {
        unsigned char cnt = 0;
        for (idx_t t = 0; t < S; ++t) {
            if (allowed_transition(s, t))
                adj_next[s][cnt++] = (unsigned char)t;
        }
        adj_count[s] = cnt;
    }
    adj_built = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!adj_built)
        build_adjacency();

    /* state counts for the current height */
    seq_t cur[S], next[S];

    /* initial row can be anything */
    for (idx_t i = 0; i < S; ++i)
        cur[i] = 1;

    /* propagate over n transitions (rows beyond the first) */
    for (idx_t step = 0; step < n; ++step) {
        for (idx_t i = 0; i < S; ++i)
            next[i] = 0;

        for (idx_t s = 0; s < S; ++s) {
            seq_t cs = cur[s];
            if (cs == 0)
                continue;
            unsigned char *nbrs = adj_next[s];
            unsigned char     m   = adj_count[s];
            for (idx_t k = 0; k < m; ++k) {
                idx_t t = nbrs[k];
                next[t] = next[t] + cs;
            }
        }

        /* swap cur and next for the next iteration */
        for (idx_t i = 0; i < S; ++i)
            cur[i] = next[i];
    }

    /* total number of arrays = sum over all final states */
    seq_t total = 0;
    for (idx_t i = 0; i < S; ++i)
        total = total + cur[i];

    return total;
}
