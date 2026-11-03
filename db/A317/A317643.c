/*
 * A317643  Expansion of theta_3(q^3)*theta_3(q^4), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A317643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to 3*x^2 + 4*y^2 = n
 * Range:     u8 n <= 836, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 8609 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t total = 0;
    /* maximal |x| is floor(sqrt(n/3)) */
    idx_t maxX = (idx_t)floor_sqrt((seq_t)(n / 3));
    for (idx_t x = 0; x <= maxX; ++x) {
        seq_t xx = (seq_t)x * (seq_t)x;
        seq_t m  = (seq_t)n - (seq_t)3 * xx;
        if (m % 4 != 0) continue;
        seq_t t = m / 4;
        seq_t y = floor_sqrt(t);
        if (y * y == t) {
            seq_t mult = (x == 0 ? 1 : 2);
            mult *= (y == 0 ? 1 : 2);
            total += mult;
        }
    }
    return total;
}
