/*
 * A342285  Coordination sequence with respect to the central vertex of a
 *          dodecagon-based tiling of the plane by copies of a certain
 *          Goldberg quadrilateral tile.
 * https://oeis.org/A342285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*A138591(n-1) for n > 1; a(0) = 1, a(1) = 6
 * Range:     u8 n <= 38, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   A138591
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A138591.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t t = A138591(n - 1);
    return t * (seq_t)6;
}
