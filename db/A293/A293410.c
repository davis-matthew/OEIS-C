/*
 * A293410  Least integer k such that k/n^2 > sqrt(3).
 * https://oeis.org/A293410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(sqrt(3)*n^2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 959 (2026-11-01)
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
    if (n == 0)
        return 0;
    seq_t t = (seq_t)n;
    seq_t n4 = t * t * t * t;        /* n^4 */
    seq_t prod = (seq_t)3 * n4;      /* 3 * n^4 */
    seq_t k = floor_sqrt(prod);      /* floor(sqrt(3)*n^2) */
    return k + 1;
}
