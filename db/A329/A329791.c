/*
 * A329791  a(n) = floor(sqrt(2)*n) + floor(sqrt(3)*n).
 * https://oeis.org/A329791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2)*n) + floor(sqrt(3)*n)
 * Range:     u8 n <= 81, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sqrt2_floor, sqrt3_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt2_floor.h"
#include "../utils/sqrt3_floor.h"

seq_t a(idx_t n)
{
    return sqrt2_floor(n) + sqrt3_floor(n);
}
