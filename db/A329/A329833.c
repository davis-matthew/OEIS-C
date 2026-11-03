/*
 * A329833  Beatty sequence for (5+sqrt(73))/8.
 * https://oeis.org/A329833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(5+sqrt(73))/8)
 * Range:     u8 none, u16 n <= 14, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
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
    /* Convert index to seq_t */
    seq_t big_n = (seq_t)n;

    /* Compute floor(n * sqrt(73)) as floor(sqrt(73 * n^2)) */
    seq_t sq = big_n * big_n;               /* n^2          */
    seq_t arg = 73 * sq;                    /* 73 * n^2     */
    seq_t sqrt_floor = floor_sqrt(arg);     /* floor(sqrt)  */

    /* Compute floor((5n + floor(n*sqrt73))/8) */
    seq_t numerator = 5 * big_n + sqrt_floor;
    return numerator / 8;
}
