/*
 * A394869  a(n) = floor(n*(sqrt(n - 1) + 2)/2).
 * https://oeis.org/A394869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(n-1)+2)/2)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-01)
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
    /* a(0) is defined as 0 */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Compute n*n*(n-1) safely as seq_t values */
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2   */
    seq_t nm1 = (seq_t)(n - (idx_t)1);      /* n-1   */
    seq_t prod = nn * nm1;                  /* n^2*(n-1) */

    /* floor(n*sqrt(n-1)/2) = floor(floor_sqrt(prod)/2) */
    seq_t sqrt_prod = floor_sqrt(prod);
    seq_t half = sqrt_prod / (seq_t)2;

    /* a(n) = n + floor(n*sqrt(n-1)/2) */
    return (seq_t)n + half;
}
