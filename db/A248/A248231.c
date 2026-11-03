/*
 * A248231  Least k such that zeta(5) - Sum_{h = 1..k} 1/h^5 < 1/n^4.
 * https://oeis.org/A248231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( sqrt(n^2/2 - 1) + 1/2 )
 * Range:     u8 n <= 5, u16 n <= 68, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 84 terms through n = 1864 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the algebraic transformation: a(n) = ( floor_sqrt(2*n*n - 4) + 1 ) / 2  for n > 1, 
   and a(1) = 1. This follows from floor( sqrt(n^2/2 - 1) + 1/2 ) = 
   floor( (sqrt(2*n*n - 4) + 1) / 2 ).  */

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
    /* handle offset */
    if (n == 1) return 1;

    /* compute t = 2*n*n - 4 safely as seq_t */
    seq_t nn = (seq_t)n;
    seq_t t = 2 * nn * nn - (seq_t)4;

    /* integer square root of t */
    seq_t s = floor_sqrt(t);

    /* result = (s + 1) / 2 */
    return (s + (seq_t)1) / (seq_t)2;
}
