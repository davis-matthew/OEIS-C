/*
 * A279612  Number of ways to write n = x^2 + y^2 + z^2 + w^2 with x + 2*y -
 *          2*z a power of 4 (including 4^0 = 1), where x,y,z,w are
 *          nonnegative integers.
 * https://oeis.org/A279612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x^2 + y^2 + z^2 + w^2 with
 *            non‑negative integers x, y, z, w and with x + 2*y - 2*z a power
 *            of 4 (1, 4, 16, …). The function enumerates all possibilities
 *            and counts the valid quadruples
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Test whether val is 1 or a power of 4. */
static inline bool is_power_of_four(idx_t val)
{
    if (val == 0) return false;
    while ((val % 4) == 0) val /= 4;
    return val == 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t count = 0;

    /* maximum x with x^2 <= n */
    idx_t max_x = (idx_t)floor_sqrt((seq_t)n);
    for (idx_t x = 0; x <= max_x; ++x)
    {
        idx_t x2   = x * x;
        idx_t rem1 = n - x2;
        idx_t max_y = (idx_t)floor_sqrt((seq_t)rem1);
        for (idx_t y = 0; y <= max_y; ++y)
        {
            idx_t y2   = y * y;
            idx_t rem2 = rem1 - y2;
            idx_t max_z = (idx_t)floor_sqrt((seq_t)rem2);
            idx_t base_xy = x + 2 * y;
            for (idx_t z = 0; z <= max_z; ++z)
            {
                idx_t z2   = z * z;
                idx_t rem3 = rem2 - z2;
                int64_t e = (int64_t)base_xy - (int64_t)2 * (int64_t)z;
                if (e <= 0) continue;
                if (!is_power_of_four((idx_t)e)) continue;

                idx_t w = (idx_t)floor_sqrt((seq_t)rem3);
                if (w * w == rem3) ++count;
            }
        }
    }

    return (seq_t)count;
}
