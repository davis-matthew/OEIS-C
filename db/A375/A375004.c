/*
 * A375004  Number of ordered primitive solutions (x,y,z,w) to x*y + x*z + x*w
 *          + y*z + y*w + z*w = n with x,y,z,w >= 1.
 * https://oeis.org/A375004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered primitive quadruples (x,y,z,w) of
 *            positive integers such that x*y + x*z + x*w + y*z + y*w + z*w =
 *            n
 * Range:     u8 n <= 160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;            /* smallest sum is 6 (1,1,1,1) */

    seq_t count = (seq_t)0;

    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; x * y <= n; ++y) {
            for (idx_t z = 1; ; ++z) {
                /* partial sum of products not including w */
                idx_t sum_xy = x * y;
                idx_t sum_xz = x * z;
                idx_t sum_yz = y * z;
                idx_t lhs = sum_xy + sum_xz + sum_yz;

                if (lhs >= n) break;           /* numerator would be <= 0 */

                idx_t denom = x + y + z;
                idx_t numer = n - lhs;

                if (numer % denom != 0) continue;
                idx_t w = numer / denom;
                if (w < 1) continue;

                /* gcd check for primitiveness */
                seq_t g = gcd((seq_t)x, (seq_t)y);
                g = gcd(g, (seq_t)z);
                g = gcd(g, (seq_t)w);

                if (g == (seq_t)1) {
                    count += (seq_t)1;
                }
            }
        }
    }

    return count;
}
