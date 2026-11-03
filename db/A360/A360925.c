/*
 * A360925  Smallest number of moves needed to win Integer Lunar Lander from
 *          starting position (n,0).
 * https://oeis.org/A360925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1+floor(sqrt(2*n^2-2*n-3)) for n>=2; a(0)=0, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 78, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Validated: standard, pass, 77 terms through n = 249 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)2 * nn * nn - (seq_t)2 * nn - (seq_t)3;
    seq_t s = floor_sqrt(val);
    return nn + (seq_t)1 + s;
}
