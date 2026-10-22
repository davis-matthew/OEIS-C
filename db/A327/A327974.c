/*
 * A327974  a(n) = A051023(n) XOR A051023(n-1), where A051023 gives the middle
 *          column of rule-30 1-D cellular automaton, when started from a lone
 *          1 cell.
 * https://oeis.org/A327974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051023(n) XOR A051023(n-1)
 * Range:     u8 n <= 9572, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051023(n) is the central cell of rule‑30 cellular automaton starting from a single 1. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety: the offset of the OEIS sequence is 1 */
        return 0;

    /* Total width needed after the last step.
       After n generations the active cells lie in [-n, n],
       we add a few extra cells for the boundary conditions.  */
    idx_t width = 2 * n + 3;
    uint8_t *prev = (uint8_t *)calloc((size_t)width, 1);
    uint8_t *curr = (uint8_t *)calloc((size_t)width, 1);
    if (!prev || !curr) {
        free(prev);
        free(curr);
        return 0;
    }

    /* Place the initial lone 1 at position center.  */
    idx_t center = n + 1;          /* safe offset so that all indices are valid */
    prev[center] = 1;              /* A051023(0) */

    seq_t last = 1;                /* holds A051023(t-1) */
    seq_t cur  = 0;                /* placeholder for current central value */

    idx_t start = center;          /* inclusive index of the leftmost active cell */
    idx_t end   = center;          /* inclusive index of the rightmost active cell */

    for (idx_t t = 1; t <= n; ++t) {
        idx_t newStart = start - 1;
        idx_t newEnd   = end   + 1;

        /* Build the next generation in curr */
        for (idx_t j = newStart; j <= newEnd; ++j) {
            uint8_t l = prev[j - 1];
            uint8_t c = prev[j];
            uint8_t r = prev[j + 1];
            curr[j] = l ^ (c | r);   /* Rule‑30 update formula */
        }

        cur = curr[center];           /* A051023(t) */

        if (t == n) {                 /* final step: compute a(n) */
            seq_t result = cur ^ last;
            free(prev);
            free(curr);
            return (seq_t)result;
        }

        /* Prepare for next iteration */
        last = cur;
        start = newStart;
        end   = newEnd;

        /* Swap the buffers */
        uint8_t *tmpp = prev;
        prev = curr;
        curr = tmpp;
    }

    /* This point is never reached. */
    free(prev);
    free(curr);
    return 0;
}
