/*
 * A340507  a(n) = floor(sqrt(2*n)) - A003056(n).
 * https://oeis.org/A340507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*n)) - floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 1549, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 111 terms through n = 8657 (2026-11-01)
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
    seq_t tn = (seq_t)n;                    /* convert index to seq_t type */
    seq_t s1 = floor_sqrt(tn * 2);          /* floor(sqrt(2*n))            */
    seq_t t  = floor_sqrt(tn * 8 + 1);      /* floor(sqrt(8*n+1))          */
    seq_t idx = (t - 1) / 2;                /* floor((sqrt(8*n+1)-1)/2)   */
    return s1 - idx;
}
