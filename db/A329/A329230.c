/*
 * A329230  The number of steps it takes to reach all n positions around a
 *          circle during the grasshopper procedure.
 * https://oeis.org/A329230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: simulate the grasshopper procedure as defined in A329230.
   The grasshopper starts at position 0 (any starting position gives the
   same number of steps due to symmetry).  After the k‑th hop it looks at
   the positions k steps clockwise and counter‑clockwise and hops to the
   one that has been visited fewer times; if both have the same number of
   visits it hops clockwise.  The initial random choice of the first
   position counts as hop 1.  The function returns the number of hops
   needed to have visited all n positions. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the offset. */
    /* allocate array holding visit counts per position */
    seq_t *vis = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (vis == NULL) {
        return 0; /* allocation failed – return 0 as sentinel */
    }
    for (idx_t i = 0; i < n; ++i) {
        vis[i] = 0;
    }

    idx_t pos = 0;          /* start at position 0 */
    seq_t steps = 1;        /* first (random) hop counts as step 1 */
    vis[pos] = 1;
    idx_t visited = 1;      /* number of distinct positions visited */

    while (visited < n) {
        ++steps;                                          /* perform next hop */
        idx_t dist = (idx_t)((steps - 1) % (seq_t)n);      /* distance to look at */

        idx_t next;
        if (dist == 0) {
            next = pos;          /* both candidate positions are the same */
        } else {
            idx_t cw  = (pos + dist) % n;
            idx_t ccw = (pos + n - dist) % n;

            /* choose the less‑visited position; tie → clockwise */
            if (vis[cw] < vis[ccw]) {
                next = cw;
            } else if (vis[ccw] < vis[cw]) {
                next = ccw;
            } else {
                next = cw;
            }
        }

        if (vis[next] == 0) {
            ++visited;
        }
        ++vis[next];
        pos = next;
    }

    free(vis);
    return steps;
}
