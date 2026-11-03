/*
 * A354329  Triangular number nearest to the sum of the first n positive
 *          triangular numbers.
 * https://oeis.org/A354329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (t^2 + t) / 2, where t = floor(sqrt(n * (n + 1) * (n + 2)
 *            / 3))
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-01)
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
    /* Convert the index to seq_t and compute the needed value safely */
    seq_t N = (seq_t)n;
    seq_t num = N * (N + 1) * (N + 2);
    seq_t div = num / (seq_t)3;          /* integer division */
    seq_t t   = floor_sqrt(div);         /* floor of the square root */
    seq_t res = (t * t + t) / (seq_t)2;  /* (t^2 + t) / 2 */
    return res;
}
