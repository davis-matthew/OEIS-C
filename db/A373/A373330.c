/*
 * A373330  a(n) is the difference between T = A000217(n^2) and the greatest
 *          square not exceeding T.
 * https://oeis.org/A373330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n^2*(n^2+1)/2 - floor_sqrt(n^2*(n^2+1)/2)^2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 594, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Validated: standard, pass, 71 terms through n = 9567 (2026-11-01)
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
    seq_t n_sq = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t T    = n_sq * (n_sq + (seq_t)1) / 2; /* triangular number of n^2 */
    seq_t s    = floor_sqrt(T);                /* greatest integer sqrt <= sqrt(T) */
    return T - s * s;                          /* difference from the next lower square */
}
