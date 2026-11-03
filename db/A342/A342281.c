/*
 * A342281  A bisection of A001951: a(n) = A001951(2*n+1).
 * https://oeis.org/A342281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n+1)*sqrt(2))
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented using integer arithmetic: a(n) = floor(sqrt(2*(2*n+1)^2)) */
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
    /* m = 2*n + 1 */
    seq_t m = (seq_t)n * 2 + 1;

    /* temp = 2 * m * m  (i.e., 2*(2*n+1)^2) */
    seq_t temp = m * m;
    temp = temp * 2;

    /* a(n) = floor(sqrt(temp)) */
    return floor_sqrt(temp);
}
