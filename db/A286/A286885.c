/*
 * A286885  Number of ways to write 6*n+1 as x^2 + 3*y^2 + 54*z^2 with x,y,z
 *          nonnegative integers.
 * https://oeis.org/A286885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions (x,y,z) to 6*n+1
 *            = x^2 + 3*y^2 + 54*z^2
 * Range:     u8 n <= 10, u16 n <= 2427, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 7705 (2026-11-01)
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
    seq_t M = (seq_t)6 * (seq_t)n + 1;
    seq_t count = 0;

    for (idx_t z = 0; ; ++z) {
        seq_t z_sq = (seq_t)z * (seq_t)z;
        if (z_sq > M / 54) break;                     /* 54*z^2 > M */
        seq_t rem_z = M - (seq_t)54 * z_sq;

        for (idx_t y = 0; ; ++y) {
            seq_t y_sq = (seq_t)y * (seq_t)y;
            if (y_sq > rem_z / 3) break;               /* 3*y^2 > rem_z */
            seq_t rem_xy = rem_z - (seq_t)3 * y_sq;

            seq_t x = floor_sqrt(rem_xy);
            if (x * x == rem_xy) ++count;              /* x^2 = rem_xy */
        }
    }

    return count;
}
