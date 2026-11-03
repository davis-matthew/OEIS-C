/*
 * A293407  Least integer k such that k/n^2 > (-1 + sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A293407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 954 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ceil((sqrt(5)-1)/2 * n^2)
 *
 *  The value is computed as
 *        a(n) = 1 + floor( (floor(n^2 * sqrt(5)) - n^2) / 2 )
 *  for n > 0.  For n = 0 the result is 0.
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
    if (n == 0) return 0;

    seq_t sn = (seq_t)n;          /* n as seq_t */
    seq_t sq = sn * sn;           /* n^2 */

    /* Compute floor(n^2 * sqrt(5)) = floor(sqrt(5 * n^4)) */
    seq_t t5 = 5 * sq * sq;       /* 5 * n^4 */
    seq_t U  = floor_sqrt(t5);    /* floor(sqrt(5 * n^4)) */

    /* floor((U - n^2) / 2)  is floor(r * n^2) */
    seq_t temp = (U - sq) / 2;
    return temp + 1;
}
