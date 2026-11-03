/*
 * A376573  Numbers that are not squares of triangular numbers. Complement of
 *          A000537.
 * https://oeis.org/A376573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+m if 4*n > m*(m-1)*(m^2+3*m+4) otherwise a(n) = n+m-1,
 *            where m = floor((4*n)^(1/4))
 * Range:     u8 n <= 15, u16 n <= 3923, u32 n <= 11011, u64 n <= 11011,
 *            u128 n <= 11011, big any n
 * Validated: standard, pass, 83 terms through n = 9306 (2026-11-01)
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
    seq_t ns   = (seq_t)n;                 /* convert index to seq_t */
    seq_t four = ns * 4;                   /* 4 * n */

    /* m = floor((4*n)^(1/4)) = floor(sqrt(floor(sqrt(4*n)))) */
    seq_t m = floor_sqrt(floor_sqrt(four));

    /* RHS = m * (m-1) * (m^2 + 3*m + 4) */
    seq_t m_sq  = m * m;
    seq_t term  = m_sq + (seq_t)3 * m + (seq_t)4;
    seq_t rhs   = m * (m - 1) * term;

    /* Return according to the comparison */
    if (four > rhs)
        return ns + m;
    else
        return ns + m - 1;
}
