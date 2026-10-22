/*
 * A360924  Smallest number of moves needed to win Integer Lunar Lander with
 *          starting position (0,n).
 * https://oeis.org/A360924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + floor(sqrt(4*n-3)) for n>0, a(0)=0
 * Range:     u8 n <= 16, u16 n <= 484, u32 n <= 484, u64 n <= 484,
 *            u128 n <= 484, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = (seq_t)n * 4 - 3;
    seq_t s = floor_sqrt(val);
    return s + 1;
}
