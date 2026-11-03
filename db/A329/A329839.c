/*
 * A329839  Beatty sequence for (-1+sqrt(41))/4.
 * https://oeis.org/A329839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * ((sqrt(41) - 1) / 4) )
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
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

seq_t a(idx_t n) {
    /* Convert n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute floor( n * sqrt(41) ) as floor_sqrt(41 * n^2) */
    seq_t tmp = nn * nn;                 /* n^2 */
    tmp = tmp * (seq_t)41;               /* 41 * n^2 */
    seq_t s = floor_sqrt(tmp);           /* floor(n * sqrt(41)) */

    /* a(n) = floor( (s - n) / 4 ) */
    seq_t diff   = s - nn;
    seq_t result = diff / (seq_t)4;

    return result;
}
