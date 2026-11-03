/*
 * A333884  Difference between smallest cube > n and n.
 * https://oeis.org/A333884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(1/3)+1)^3 - n
 * Range:     u8 n <= 63, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 97 terms through n = 9808 (2026-11-01)
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
    /* Find the largest integer m with m^3 <= n */
    seq_t m = floor_cube_root((seq_t)n);
    /* Next cube is (m+1)^3 */
    seq_t m1 = m + (seq_t)1;
    seq_t cube = m1 * m1 * m1;
    return cube - (seq_t)n;
}
