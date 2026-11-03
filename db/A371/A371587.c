/*
 * A371587  a(n) is the number of integers m from 1 to n inclusive such that
 *          m^m is a cube.
 * https://oeis.org/A371587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/3) + floor(n^(1/3)) - floor(n^(1/3)/3)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 911 (2026-11-01)
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
    /* Convert index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* floor(n/3) using the index type, then cast to seq_t */
    seq_t div_three = (seq_t)(n / 3);

    /* floor of the cube root of n */
    seq_t cube_root = floor_cube_root(n_seq);

    /* floor(cube_root / 3) */
    seq_t cube_root_div_three = cube_root / 3;

    /* Compute the formula */
    return div_three + cube_root - cube_root_div_three;
}
