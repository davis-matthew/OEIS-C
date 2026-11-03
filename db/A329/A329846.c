/*
 * A329846  Beatty sequence for (7+sqrt(29))/5.
 * https://oeis.org/A329846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(7+sqrt(29))/5)
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
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
    if (n == 0) return 0;          /* undefined below offset, guard */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;          /* n^2 */
    prod *= 29;                    /* 29*n^2 */
    seq_t t = floor_sqrt(prod);    /* floor(n * sqrt(29)) */
    seq_t res = (7 * nn + t) / 5;  /* floor((7n + t)/5) */
    return res;
}
