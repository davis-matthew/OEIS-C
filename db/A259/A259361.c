/*
 * A259361  n occurs 2n+2 times.
 * https://oeis.org/A259361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((-1 + sqrt(1+4*n))/2)
 * Range:     u8 n <= 15, u16 n <= 3588, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 4 + 1;
    seq_t r = floor_sqrt(t);
    return (r - 1) / 2;
}
