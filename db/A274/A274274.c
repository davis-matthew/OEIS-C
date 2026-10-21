/*
 * A274274  Number of ordered ways to write n as x^3 + y^2 + z^2, where x,y,z
 *          are nonnegative integers with y <= z.
 * https://oeis.org/A274274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered representations n = x³ + y² + z² with
 *            non‑negative integers x , y , z and constraint y ≤ z
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icuberoot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/icuberoot.h"
/* Integer square root (floor).  Uses only integer arithmetic. */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    /* The maximum possible sqrt of a 64‑bit unsigned integer is < 2³². */
    idx_t high = (idx_t)1 << 32;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;     /* upper mid to avoid infinite loop */
        if (mid == 0 || mid <= n / mid)        /* mid² ≤ n  (no overflow) */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t max_x = icuberoot(n);

    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t x3 = x * x * x;        /* x³  (safe: x ≤ 2²¹) */
        idx_t rem = n - x3;          /* remaining value for y² + z² */

        idx_t max_y = isqrt(rem);
        for (idx_t y = 0; y <= max_y; ++y) {
            idx_t y2 = y * y;
            idx_t z2 = rem - y2;

            idx_t z = isqrt(z2);
            if (z * z == z2 && y <= z) {
                ++count;              /* valid ordered triple (x, y, z) */
            }
        }
    }
    return count;
}
