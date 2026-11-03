/*
 * A329844  Beatty sequence for (11+sqrt(61))/6.
 * https://oeis.org/A329844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(11 + sqrt(61))/6)
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
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
    /* For n = 0 this is outside the OEIS offset; return 0. */
    if (n == 0) return 0;

    /* Compute floor(n * sqrt(61)) as floor_sqrt(61 * n^2). */
    seq_t nsq = (seq_t)n * (seq_t)n;        /* n^2 */
    seq_t v   = nsq * (seq_t)61;            /* 61 * n^2 */
    seq_t sq  = floor_sqrt(v);              /* floor(n*sqrt(61)) */

    /* a(n) = floor((11*n + floor(n*sqrt(61))) / 6) */
    seq_t res = ((seq_t)11 * (seq_t)n + sq) / 6;
    return res;
}
