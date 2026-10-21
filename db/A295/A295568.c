/*
 * A295568  Irregular triangle, read by rows: the Catalan generating tree,
 *          read from left to right, row by row, starting at the root.
 * https://oeis.org/A295568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = label of the n-th node in the breadth‐first * traversal
 *            of the Catalan generating tree. * A(n) is obtained by finding
 *            the row L such that * S(L-1) < n ≤ S(L), S(k) = Σ_{i=1..k}
 *            Catalan(i), * computing the index j of the node in that row
 *            (0‑based), * then descending the tree from the root k...
 * Range:     u8 n <= 9, u16 n <= 1933, u32 n <= 9696, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n=0 */

    /* Determine the row (level) containing the n‑th term. */
    seq_t catalan = 1;                 /* Catalan(1) */
    idx_t level   = 1;                 /* current row number (1‑based) */
    seq_t cumulative = 0;              /* Σ_{i=1}^{level-1} Catalan(i) */

    while ((cumulative + catalan) < (seq_t)n) {
        cumulative += catalan;
        ++level;
        /* Catalan(level) = binom(2·level, level) / (level+1) */
        catalan = binom(2*level, level) / (level+1);
    }

    /* Index within the found row (0‑based). */
    idx_t idx_in_row = (idx_t)((seq_t)n - cumulative - 1);

    /* Special case: root row. */
    if (level == 1)
        return (seq_t)2;

    /* Prepare table F[h][k] where
       h = 0 .. level-1  (distance below the node)
       k = 2 .. 2·level   (possible labels, enough for all needed children).
       F[0][k] = 1 for all k.                */
    size_t h_max = (size_t)(level - 1);
    size_t max_k = (size_t)(2 * level);    /* sufficient for all child values */
    size_t stride = max_k + 1;
    seq_t *F = (seq_t *)malloc((h_max + 1) * stride * sizeof(seq_t));
    if (!F)
        return 0;                        /* allocation failure */

    /* F[0][k] = 1 for k ≥ 2. */
    for (size_t k = 2; k <= max_k; ++k)
        F[k] = (seq_t)1;

    /* Build the table for h = 1 .. h_max. */
    for (size_t h = 1; h <= h_max; ++h) {
        size_t base  = h     * stride;
        size_t prev  = (h-1) * stride;
        for (size_t k = 2; k <= max_k; ++k) {
            seq_t sum = 0;
            for (size_t child = 2; child <= k+1 && child <= max_k; ++child)
                sum += F[prev + child];
            F[base + k] = sum;
        }
    }

    /* Descend the tree to find the label of the node. */
    seq_t current_k = 2;          /* root label */
    size_t depth_rem = level - 1; /* edges left to reach the target row */
    idx_t  idx = idx_in_row;

    while (depth_rem > 0) {
        for (size_t child = 2; child <= (size_t)current_k + 1; ++child) {
            seq_t count = F[(depth_rem-1) * stride + child];
            if ((idx_t)idx < count) {   /* target is within this child */
                current_k = (seq_t)child;
                --depth_rem;
                break;
            }
            idx -= (idx_t)count;        /* skip this child's subtree */
        }
    }

    free(F);
    return current_k;
}
