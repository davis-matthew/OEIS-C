/*
 * A327177  a(n) = [(2n+1)r] - [(n+1)r] - [nr], where [ ] = floor and r =
 *          sqrt(2).
 * https://oeis.org/A327177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+1)*√2) - floor((n+1)*√2) - floor(n*√2)
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9371 (2026-11-01)
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

static inline seq_t floor_sqrt2_mult(idx_t k)
{
    /* Compute floor(k * √2) as floor(√(2 * k * k)) */
    return floor_sqrt((seq_t)2 * (seq_t)k * (seq_t)k);
}

seq_t a(idx_t n)
{
    seq_t s_n       = floor_sqrt2_mult(n);          /* floor(n √2)          */
    seq_t s_n_plus1 = floor_sqrt2_mult(n + 1);      /* floor((n+1) √2)      */
    seq_t s_2n_plus1 = floor_sqrt2_mult(2 * n + 1); /* floor((2n+1) √2)     */

    return s_2n_plus1 - s_n_plus1 - s_n;
}
