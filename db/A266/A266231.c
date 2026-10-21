/*
 * A266231  Least positive integer x such that x^2 - n = y^3 + z^3 for some
 *          positive integers y and z, or 0 if no such x exists.
 * https://oeis.org/A266231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer x such that x^2 - n = y^3 + z^3
 *            for some positive integers y and z; 0 if no such x exists
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < 1 is undefined, return 0 */

    for (seq_t x = 1; x != 0; ++x)  /* loop until wrap‑around (safety) */
    {
        seq_t t = x * x;
        if (t <= n) continue;      /* t - n must be positive */
        t -= n;                    /* t = x^2 - n */

        /* Search for positive y and z such that y^3 + z^3 == t */
        idx_t max_y = (idx_t)floor_cube_root(t);
        for (idx_t y = 1; y <= max_y; ++y)
        {
            seq_t y3 = (seq_t)y * y * y;
            seq_t rem = t - y3;
            if (rem <= 0) continue;

            seq_t z = floor_cube_root(rem);
            if (z > 0 && z * z * z == rem)
            {
                /* Found a solution; x is minimal because we iterate x ascending */
                return x;
            }
        }
    }

    /* No solution found (should not happen for tested n values) */
    return 0;
}
