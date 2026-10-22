/*
 * A321667  (3/2) * n^(2/3) rounded to nearest integer.
 * https://oeis.org/A321667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((3/2) * n^(2/3))
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 5345, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* If the index is 0, return 0 (out of the OEIS range). */
    if (n == 0) return 0;

    /* Compute n^2 as a seq_t. */
    seq_t nn = (seq_t)n * (seq_t)n;      /* n^2 */

    /* lhs = 27 * n^2  (equivalent to (3 * n^(2/3))^3). */
    seq_t lhs = nn * 27;

    /* y = floor_cube_root(lhs) = floor(3 * n^(2/3)). */
    seq_t y = floor_cube_root(lhs);

    /* Floor of (3/2) * n^(2/3). */
    seq_t a_floor = y / 2;               /* integer division */

    /* Determine if we should round up. */
    seq_t r = a_floor * 2 + 1;           /* 2*a_floor + 1 */
    seq_t r3 = r * r * r;                /* (2*a_floor + 1)^3 */

    if (lhs >= r3)
        return a_floor + 1;              /* round up */
    else
        return a_floor;                  /* keep floor value */
}
