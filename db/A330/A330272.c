/*
 * A330272  a(n) is the least nonnegative integer k such that n OR k is a cube
 *          (where OR denotes the bitwise OR operator).
 * https://oeis.org/A330272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that (n | k) is a perfect cube, i.e. the
 *            smallest cube m with (m & n) == n and k = m & ~n
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 2005, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 72 terms through n = 7911 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    /* Find the largest integer r with r³ ≤ s. */
    seq_t r = floor_cube_root(s);
    seq_t c = r * r * r;
    /* If n itself is a perfect cube, k = 0. */
    if (c == s) {
        return (seq_t)0;
    }
    /* Search upward for the first cube that covers all bits of n. */
    for (seq_t x = r + 1; ; ++x) {
        c = x * x * x;
        if ((c & s) == s) {
            /* The minimal k is the part of the cube not present in n. */
            return c & ~s;
        }
    }
}
