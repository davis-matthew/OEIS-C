/*
 * A278814  a(n) = ceiling(sqrt(3n+1)).
 * https://oeis.org/A278814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(3*n + 1))
 * Range:     u8 n <= 20, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the OEIS sequence A278814: a(n) = ceil(sqrt(3n+1)). */

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
    /* Compute 3*n + 1 in seq_t arithmetic. */
    const seq_t v = (seq_t)3 * (seq_t)n + 1;

    /* floor_sqrt returns the greatest integer s with s*s <= v */
    const seq_t s = floor_sqrt(v);

    /* If s*s equals v, v is a perfect square; otherwise
       the next integer is the ceil of the square root. */
    return (s * s == v) ? s : s + 1;
}
