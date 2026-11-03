/*
 * A385071  a(n) = number of triangular numbers <= n-th tetrahedral number.
 * https://oeis.org/A385071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3 + sqrt(9 + 12*n*(n+1)*(n+2)))/6)
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
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
    seq_t nn   = (seq_t)n;                 /* cast index to seq_t */
    seq_t prod = nn * (nn + 1) * (nn + 2); /* n*(n+1)*(n+2)              */
    seq_t u    = 9 + (prod * 12);          /* 9 + 12*n*(n+1)*(n+2)       */
    seq_t s    = floor_sqrt(u);            /* floor(sqrt(u))              */
    return (3 + s) / 6;                    /* floor((3+sqrt(u))/6)       */
}
