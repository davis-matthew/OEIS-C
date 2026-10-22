/*
 * A379414  a(n) = n + floor(n*s/r) + floor(n*t/r), where r = 3^(1/4), s =
 *          3^(1/2), t = 3^(3/4).
 * https://oeis.org/A379414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*3^(1/4)) + floor(n*sqrt(3))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* floor(n * sqrt(3)) = floor( sqrt(n^2 * 3) ) */
    seq_t n2 = sn * sn;
    seq_t floor_n_sqrt3 = sqrt_floor(n2 * 3);

    /* floor(n * 3^(1/4)) = floor( (n^4 * 3)^(1/4) ) */
    seq_t n4 = n2 * n2;              /* n^4 */
    seq_t tmp = sqrt_floor(n4 * 3);   /* floor( sqrt(n^4 * 3) ) */
    seq_t floor_n_r = sqrt_floor(tmp);/* floor( sqrt( floor( sqrt(n^4 * 3) ) ) ) */

    return sn + floor_n_r + floor_n_sqrt3;
}
