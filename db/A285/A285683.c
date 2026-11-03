/*
 * A285683  Positions of 2 in A285680.
 * https://oeis.org/A285683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor(n*phi) - n + 3, where phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9796 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The integer part of n*phi can be computed as floor((n + floor(n*sqrt5))/2).
   floor(n*sqrt5) equals floor_sqrt(5*n*n).  */
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
    /* For safety, though the OEIS offset is 1, handle n==0 trivially. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute floor(n*sqrt5) = floor_sqrt(5*n*n) */
    seq_t nn = (seq_t)n * (seq_t)n;      /* n^2 */
    seq_t prod = (seq_t)5 * nn;          /* 5*n^2 */
    seq_t floor_sqrt5 = floor_sqrt(prod);/* floor(n*sqrt5) */

    /* floor(n*phi) = floor((n + floor_sqrt5)/2) */
    seq_t sum = (seq_t)n + floor_sqrt5;
    seq_t floor_nphi = sum / 2;

    /* a(n) = 3*floor(n*phi) - n + 3 */
    seq_t result = floor_nphi * 3 - (seq_t)n + 3;
    return result;
}
