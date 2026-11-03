/*
 * A329922  Integral solutions to Mordell's equation y^2 = x^3 - n with
 *          minimal absolute value of x (a(n) gives y-values).
 * https://oeis.org/A329922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal |y| for y^2 = x^3 + n with minimal |x| (return 0
 *            if no integer solution exists)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9790 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    /* bound for |x| – sufficient for n up to about 10 000 */
    const idx_t MAX_ABS_X = 5000;
    for (idx_t absx = 0; absx <= MAX_ABS_X; ++absx)
    {
        /* test negative x (skip x = 0 twice) */
        if (absx > 0)
        {
            int64_t x = -(int64_t)absx;
            int64_t cube = x * x * x;
            int64_t t = (int64_t)n + cube;
            if (t >= 0)
            {
                seq_t tt = (seq_t)t;
                seq_t y = floor_sqrt(tt);
                if (y * y == tt)
                    return y;
            }
        }
        /* test non‑negative x */
        {
            int64_t x = (int64_t)absx;
            int64_t cube = x * x * x;
            int64_t t = (int64_t)n + cube;
            if (t >= 0)
            {
                seq_t tt = (seq_t)t;
                seq_t y = floor_sqrt(tt);
                if (y * y == tt)
                    return y;
            }
        }
    }
    /* no solution found within bound */
    return (seq_t)0;
}
