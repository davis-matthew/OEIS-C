/*
 * A379697  For n >= 3, a(n) is the least k >= 0 such that (k + 1)*(2*n + k) /
 *          2 is a triangular number (A000217).
 * https://oeis.org/A379697
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k >= 0 such that (k+1)*(2*n + k)/2 is a
 *            triangular number
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9714 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  n >= 3  (behaviour for smaller n is irrelevant)  */
    seq_t ns = (seq_t)n;
    seq_t k = 0;

    for (;; ++k)
    {
        /* product P = (k+1)(2n + k) = m(m+1) */
        seq_t x = k + 1;
        seq_t y = 2 * ns + k;
        seq_t P = x * y;

        /* discriminant of m^2 + m - P = 0 */
        seq_t D = P * 4 + 1;

        seq_t s = floor_sqrt(D);
        if (s * s == D)   /* P is a pronic number, so the sum is triangular */
            return k;
    }
}
