/*
 * A341239  a(n) = floor(r*floor(s*n)), where r = 1 + sqrt(2) and s = sqrt(2).
 * https://oeis.org/A341239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1+sqrt(2))*floor(sqrt(2)*n))
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent implementation using integer square roots:        */
/*   let t = floor_sqrt(2*n*n);                                 */
/*   let u = floor_sqrt(2*t*t);                                 */
/*   return t + u;                                              */

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
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t t   = floor_sqrt((seq_t)2 * nn * nn); /* floor(sqrt(2)*n) */
    seq_t u   = floor_sqrt((seq_t)2 * t * t);   /* floor(sqrt(2)*t) */
    return t + u;
}
