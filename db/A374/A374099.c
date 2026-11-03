/*
 * A374099  Number of solutions to n^2 = x^2 + x*y + y^2 with 0 < x < y.
 * https://oeis.org/A374099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs 0 < x < y with n^2 = x^2 + x*y +
 *            y^2
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-01)
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
    seq_t nn   = (seq_t)n * (seq_t)n;          /* n^2            */
    seq_t limit = (seq_t)4 * nn;               /* 4 * n^2        */
    seq_t count = 0;                           /* result          */
    seq_t x = 1;

    /* iterate while 3*x*x <= 4*n^2 */
    for (; (seq_t)3 * x * x <= limit; ++x)
    {
        seq_t D = limit - (seq_t)3 * x * x;            /* discriminant   */
        seq_t k = floor_sqrt(D);                      /* sqrt(D) if int */

        if (k * k != D)                /* not a perfect square   */
            continue;

        /* parity: k - x must be even for integer y */
        if (((k - x) & 1) != 0)
            continue;

        /* require y > x   =>  k > 3*x  (since y = (k - x)/2) */
        if (k <= (seq_t)3 * x)
            continue;

        ++count;      /* a valid (x,y) pair found */
    }

    return count;
}
