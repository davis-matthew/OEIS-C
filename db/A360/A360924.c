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
 * Validated: standard, pass, 92 terms through n = 471 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = (seq_t)n * 4 - 3;
    seq_t s = floor_sqrt(val);
    return s + 1;
}
