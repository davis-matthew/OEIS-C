/*
 * A349662  a(n) is the number of squares strictly between n^2 and n^3.
 * https://oeis.org/A349662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 0 or 1; a(n) = floor( sqrt(n^3 - 1) ) - n for n
 *            > 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 992, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9018 (2026-11-01)
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
    if (n <= 1) {
        return 0;
    }
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t t  = n3 - 1;
    seq_t s  = floor_sqrt(t);
    return s - ns;
}
