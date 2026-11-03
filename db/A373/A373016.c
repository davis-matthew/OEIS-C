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
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-01)
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
    /* t = 3*n^2 + 2*n */
    seq_t nn   = (seq_t)n;
    seq_t t    = 3 * nn * nn + 2 * nn;

    /* s = floor(sqrt(t)) + 1 */
    seq_t sqrt_t = floor_sqrt(t);
    seq_t s      = sqrt_t + 1;

    /* minimal k = s^2 - t (guaranteed > 0) */
    return s * s - t;
}
