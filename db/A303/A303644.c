/*
 * A303644  a(n) is the number of lattice points in a Cartesian grid between a
 *          square of side length 2*n, centered at the origin, and its
 *          inscribed circle. The sides of the square are parallel to the
 *          coordinate axes.
 * https://oeis.org/A303644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A016754(n-1) - A000328(n) + 4
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts lattice points strictly between the square of side 2*n
   centered at the origin and its inscribed circle. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sq_n = (seq_t)n * (seq_t)n;          /* n^2 as seq_t           */
    seq_t max_y = (seq_t)n - 1;                /* largest y inside square */
    seq_t total_quadrant = 0;

    for (idx_t x = 1; x < n; ++x)
    {
        seq_t t = sq_n - (seq_t)x * (seq_t)x;   /* n^2 - x^2               */
        seq_t r = floor_sqrt(t);               /* ⌊√(n^2 - x^2)⌋          */
        seq_t cnt = max_y - r;                 /* points for this x          */
        total_quadrant += cnt;
    }

    return total_quadrant << 2;                /* all four quadrants        */
}
