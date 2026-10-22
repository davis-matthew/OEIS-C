/*
 * A392671  Triangle read by rows: the n-th row gives the denominators of the
 *          node values in a pre-order traversal of the Calkin-Wilf tree with
 *          n levels, 0 <= k < 2^n - 1.
 * https://oeis.org/A392671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look, frac
 * Formula:   not stated
 * Range:     u8 n <= 9374, u16 n <= 9517, u32 n <= 9517, u64 n <= 9517,
 *            u128 n <= 9517, big any n
 * Helpers:   denom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392671: denominator of the n‑th node in the preorder traversal
   of the Calkin‑Wilf tree truncated to the smallest row that contains
   the node. */

#include <stdint.h>
#include "../utils/denom.h"

/* Public API required by the build system.  It works for any n >= 1.
   For n = 0 it returns 0 (outside the official offset). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* outside offset, behaviour irrelevant */

    /* Find the smallest row depth such that the cumulative number of
       nodes up to that row is at least n.  We use __int128 so that
       the shift is safe for depths up to 64. */
    uint64_t depth = 1;
    unsigned __int128 cumulative = 0;
    while (1) {
        unsigned __int128 nodes_row = (((unsigned __int128)1) << depth) - 1;
        cumulative += nodes_row;
        if (cumulative >= n)
            break;
        depth++;
    }

    /* Number of nodes in the current row (as 128‑bit). */
    unsigned __int128 nodes_row128 = (((unsigned __int128)1) << depth) - 1;
    /* Number of nodes before this row. */
    unsigned __int128 prev_cum128 = cumulative - nodes_row128;
    uint64_t prev_cum = (uint64_t)prev_cum128;

    /* 0‑based index within the current row. */
    uint64_t idx_in_row = (uint64_t)(n - prev_cum - 1);

    /* Compute the denominator of the required node. */
    return denom(depth, (seq_t)1, (seq_t)1, idx_in_row);
}
