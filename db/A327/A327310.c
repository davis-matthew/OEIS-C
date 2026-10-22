/*
 * A327310  a(n) = floor(3*n*r) - 3*floor(n*r), where r = sqrt(8).
 * https://oeis.org/A327310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n*sqrt(8)) - 3*floor(n*sqrt(8))
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 3741, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t floor1 = floor_sqrt( (seq_t)8 * s * s );          /* floor(n*sqrt(8)) */
    seq_t s6 = s * (seq_t)6;
    seq_t floor2 = floor_sqrt( (seq_t)2 * s6 * s6 );        /* floor(3*n*sqrt(8)) */
    return floor2 - (seq_t)3 * floor1;
}
