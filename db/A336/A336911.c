/*
 * A336911  Image of n under the x^3+1 map, which is a variation of the 3x+1
 *          (Collatz) map.
 * https://oeis.org/A336911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor_sqrt(n) if n is even, n^3+1 if n is odd
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
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
    if ((n & 1) == 0)          /* n is even */
        return floor_sqrt((seq_t)n);

    /* n is odd */
    seq_t m = (seq_t)n;
    return m * m * m + 1;
}
