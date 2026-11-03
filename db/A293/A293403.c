/*
 * A293403  Greatest integer k such that k/n^2 < (3 + sqrt(5))/2.
 * https://oeis.org/A293403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(((3 + sqrt(5))/2) * n^2) Equivalently a(n) = (3*n^2
 *            + floor_sqrt(5*n^4)) / 2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 47, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 983 (2026-11-01)
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
    /* Convert index to seq_t first to avoid overflow when multiplying */
    seq_t nn   = (seq_t)n;          /* n */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t big  = n2 * n2;           /* n^4 */
    big = big * (seq_t)5;           /* 5 * n^4 */
    seq_t s = floor_sqrt(big);     /* floor(sqrt(5 * n^4)) = floor(n^2 * sqrt(5)) */
    seq_t res = ( (seq_t)3 * n2 + s ) / (seq_t)2; /* floor division by 2 */
    return res;
}
