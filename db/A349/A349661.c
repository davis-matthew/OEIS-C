/*
 * A349661  Number of ways to write n as x^4 + y^2 + (z^2 + 4^w)/2 with
 *          x,y,z,w nonnegative integers.
 * https://oeis.org/A349661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (x,y,z,w) ≥ 0 to n = x^4 + y^2 + (z^2
 *            + 4^w)/2
 * Range:     u8 n <= 31, u16 n <= 8053, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe guard – offset is 1 */
    seq_t two_n = (seq_t)n * 2ULL;  /* working with 2*n to avoid fractions */
    seq_t count = 0;

    seq_t pow4 = 1;                 /* 4^w, w starts at 0 */
    for (idx_t w = 0; pow4 <= two_n; ++w, pow4 *= 4ULL)
    {
        seq_t rem_w = two_n - pow4;   /* remaining after accounting for 4^w */

        /* loop over x: 2*x^4 <= rem_w */
        for (idx_t x = 0; ; ++x)
        {
            seq_t x2 = (seq_t)x * x;
            seq_t x4 = x2 * x2;          /* x^4 */
            seq_t termX = x4 << 1;        /* 2*x^4 */
            if (termX > rem_w) break;

            seq_t rem_xy = rem_w - termX;   /* remaining after x */

            /* loop over y: 2*y^2 <= rem_xy */
            for (idx_t y = 0; ; ++y)
            {
                seq_t y2 = (seq_t)y * y;
                seq_t termY = y2 << 1;      /* 2*y^2 */
                if (termY > rem_xy) break;

                seq_t val = rem_xy - termY;  /* must be a perfect square z^2 */
                seq_t z = floor_sqrt(val);
                if (z * z == val) ++count;
            }
        }
    }

    return count;
}
