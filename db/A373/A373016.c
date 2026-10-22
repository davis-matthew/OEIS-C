/*
 * A373016  a(n) is the least positive integer k such that 3*n^2 + 2*n + k is
 *          a square.
 * https://oeis.org/A373016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(3*n^2 + 2*n))^2 - (3*n^2 + 2*n)
 * Range:     u8 n <= 4, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* t = 3*n^2 + 2*n */
    seq_t nn   = (seq_t)n;
    seq_t t    = 3 * nn * nn + 2 * nn;

    /* s = floor(sqrt(t)) + 1 */
    seq_t sqrt_t = floor_sqrt(t);
    seq_t s      = sqrt_t + 1;

    /* minimal k = s^2 - t (guaranteed > 0) */
    return s * s - t;
}
