/*
 * A253663  Number of positive solutions to x^2+y^2+z^2 <= n^2.
 * https://oeis.org/A253663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253663:
    Number of positive integer solutions (x,y,z) with x² + y² + z² ≤ n².
    The implementation uses integer arithmetic only and runs in O(n²)
    time by updating the maximal admissible z for each (x,y) pair
    incrementally.  All intermediate values fit into 64‑bit integers for
    n ≤ 10 000.  No floating point functions are used. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(0) = a(1) = 0 */

    seq_t sum = 0;

    /* n² fits comfortably into 64‑bit unsigned integer for n ≤ 10 000 */
    uint64_t n2 = (uint64_t)n * (uint64_t)n;

    for (idx_t x = 1; x <= n; ++x) {
        uint64_t xsq = (uint64_t)x * (uint64_t)x;
        if (xsq > n2) break;          /* further x will be even larger */
        uint64_t rem_x = n2 - xsq;     /* remaining value for y²+z² */

        /* upper bound on y for this x */
        idx_t ymax = (idx_t)floor_sqrt((seq_t)rem_x);
        idx_t zmax = ymax;             /* start with the largest possible z */

        for (idx_t y = 1; y <= ymax; ++y) {
            uint64_t ysq = (uint64_t)y * (uint64_t)y;
            if (ysq > rem_x) break;    /* y is now too large */
            uint64_t rem_xy = rem_x - ysq; /* remaining value for z² */

            /* decrement zmax until zmax² ≤ rem_xy */
            while ((seq_t)zmax * zmax > rem_xy) --zmax;

            if (zmax > 0) sum += (seq_t)zmax;
            /* when zmax == 0 further y will only reduce rem_xy, so break */
            if (zmax == 0 && rem_xy == 0) break;
        }
    }

    return sum;
}
