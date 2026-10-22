/*
 * A335624  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x + 3*y +
 *          4*z a square, where x, y, z, w are nonnegative integers.
 * https://oeis.org/A335624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as * x^2 + y^2 + z^2 + w^2
 *            with nonnegative integers x, y, z, w * such that x + 3*y + 4*z
 *            is a perfect square
 * Range:     u8 n <= 4289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;      /* result type may be larger than idx_t */

    /* Outer loop over x, bounded by ⌊√n⌋ */
    for (idx_t x = 0; x * x <= n; ++x) {
        idx_t xsq = x * x;

        /* Middle loop over y, bounded by ⌊√(n – x²)⌋ */
        for (idx_t y = 0; xsq + y * y <= n; ++y) {
            idx_t xy_sum = xsq + y * y;

            /* Inner loop over z, bounded by ⌊√(n – x² – y²)⌋ */
            for (idx_t z = 0; xy_sum + z * z <= n; ++z) {
                idx_t xyz_sum = xy_sum + z * z;
                idx_t rem = n - xyz_sum;          /* must be w² */

                /* w² has to be a perfect square */
                idx_t w = _floor_isqrt_u64(rem);
                if (w * w != rem)
                    continue;

                /* x + 3*y + 4*z must be a perfect square */
                idx_t L = x + 3 * y + 4 * z;
                idx_t LS = _floor_isqrt_u64(L);
                if (LS * LS == L) {
                    /* Found one representation */
                    count += (seq_t)1;
                }
            }
        }
    }

    return count;
}
