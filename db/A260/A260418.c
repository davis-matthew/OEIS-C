/*
 * A260418  Number of ways to write 12*n+5 as 4*x^4 + 4*y^2 + z^2, where x is
 *          a nonnegative integer, and y and z are positive integers.
 * https://oeis.org/A260418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts solutions to 12*n+5 = 4*x^4 + 4*y^2 + z^2 with x ≥ 0
 *            integer, y > 0 integer, z > 0 integer
 * Range:     u8 n <= 5, u16 n <= 1089, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9966 (2026-11-01)
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
    /* target value */
    seq_t m = (seq_t)n * 12 + 5;
    seq_t count = 0;

    /* x loop: 0 ≤ x, such that 4*x^4 ≤ m-5  (minimum RHS is 5 when y=z=1) */
    for (idx_t x = 0; ; ++x) {
        seq_t x4 = (seq_t)x * x * x * x;
        seq_t termx = 4 * x4;
        if (termx > m - 5) break;          /* no further x are possible */

        seq_t M = m - termx;                /* remaining value for 4*y^2 + z^2 */

        /* y loop: 1 ≤ y with 4*y^2 < M  →  y ≤ floor(sqrt((M-1)/4)) */
        seq_t temp = (M - 1) / 4;
        idx_t y_max = (idx_t)floor_sqrt(temp);
        for (idx_t y = 1; y <= y_max; ++y) {
            seq_t four_y2 = 4 * (seq_t)y * y;
            seq_t z2 = M - four_y2;           /* z^2 */
            seq_t z   = floor_sqrt(z2);
            if (z * z == z2) ++count;         /* z ≥ 1 automatically */
        }
    }
    return count;
}
