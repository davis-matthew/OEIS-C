/*
 * A317641  Expansion of theta_3(q)*theta_3(q^10), where theta_3() is the
 *          Jacobi theta function.
 * https://oeis.org/A317641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y) of x^2 + 10*y^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9861 (2026-11-01)
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
    seq_t count = 0;

    /* maximal |y| such that 10*y^2 <= n */
    seq_t y_max = floor_sqrt((seq_t)n / 10);

    /* handle y = 0 */
    seq_t r = floor_sqrt((seq_t)n);
    if (r * r == (seq_t)n) {
        count += (r == 0) ? 1 : 2;
    }

    /* handle |y| >= 1 */
    for (idx_t y = 1; y <= (idx_t)y_max; ++y) {
        seq_t yy = (seq_t)y;
        seq_t rem = (seq_t)n - (seq_t)10 * yy * yy;
        r = floor_sqrt(rem);
        if (r * r == rem) {
            count += (r == 0) ? 2 : 4;
        }
    }

    return count;
}
