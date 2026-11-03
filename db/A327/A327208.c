/*
 * A327208  a(n) = [(2n+2)r] - [(n+2)r] - [nr], where [ ] = floor and r =
 *          sqrt(3).
 * https://oeis.org/A327208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+2)*sqrt(3)) - floor((n+2)*sqrt(3)) -
 *            floor(n*sqrt(3))
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 8494, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9862 (2026-11-01)
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
    /* floor(k*sqrt(3)) can be computed as floor_sqrt(3*k^2) */
    seq_t fn = floor_sqrt((seq_t)3 * (seq_t)n * (seq_t)n);

    idx_t n_plus_2 = n + 2;
    seq_t fp = floor_sqrt((seq_t)3 * (seq_t)n_plus_2 * (seq_t)n_plus_2);

    idx_t two_n_plus_2 = 2 * n + 2;
    seq_t f2 = floor_sqrt((seq_t)3 * (seq_t)two_n_plus_2 * (seq_t)two_n_plus_2);

    return f2 - fp - fn;
}
