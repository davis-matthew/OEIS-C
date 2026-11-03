/*
 * A329843  Beatty sequence for (1+sqrt(61))/6.
 * https://oeis.org/A329843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + floor(n * sqrt(61))) / 6)
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 66, u64 n <= 66, u128 n <= 66,
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

seq_t a(idx_t n)
{
    /* k = floor(n * sqrt(61)) = floor(sqrt(61 * n^2)) */
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t val = (seq_t)61 * n_sq;              /* 61 * n^2 */
    seq_t k   = floor_sqrt(val);               /* floor(sqrt(61 * n^2)) */

    /* a(n) = (n + k) / 6 */
    seq_t sum = (seq_t)n + k;
    return sum / (seq_t)6;
}
