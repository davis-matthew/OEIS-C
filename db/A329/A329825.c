/*
 * A329825  Beatty sequence for (3+sqrt(17))/4.
 * https://oeis.org/A329825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((3 + sqrt(17))/4) * n)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7905, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9815 (2026-11-01)
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
    /* use integer arithmetic only */
    seq_t ns = (seq_t)n;              /* n as seq_t */
    seq_t ns2 = ns * ns;              /* n^2  */
    seq_t v   = (seq_t)17 * ns2;      /* 17*n^2  */
    seq_t m   = floor_sqrt(v);        /* floor(n*sqrt(17)) */
    seq_t k   = (seq_t)3 * ns;        /* 3*n  */
    seq_t sum = k + m;                /* 3n + floor(n*sqrt(17)) */
    return sum / (seq_t)4;            /* floor((3n + floor(n*sqrt(17))) / 4) */
}
