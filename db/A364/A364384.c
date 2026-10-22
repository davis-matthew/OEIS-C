/*
 * A364384  a(n) is the number of quadratic equations u*x^2 + v*x + w = 0 with
 *          different solution sets L != {}, where n = abs(u) + abs(v) +
 *          abs(w), the coefficients u, v, w as well as the solutions x_1, x_2
 *          are integers and GCD(u, v, w) = 1.
 * https://oeis.org/A364384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of unordered integer pairs (x, y) such that * |x +
 *            y| + |x * y| = n - 1. This counts primitive quadratic equations
 *            * u x² + v x + w = 0 (u > 0, gcd(u, v, w) = 1) with integer
 *            roots * and |u| + |v| + |w| = n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset: a(1) = 1 */
    if (n == 0) return 0;          /* undefined below offset */
    idx_t m = n - 1;               /* m = |u| + |v| + |w| - 1 = |v1+v2| + |v1*v2| */
    if (m == 0) return 1;          /* only pair (0,0) */

    seq_t count = 2;               /* pairs (0, m) and (0, -m) (product 0) */

    for (idx_t abs_p = 1; abs_p <= m; ++abs_p) {
        idx_t s_abs = m - abs_p;   /* required |x + y| */

        /* Iterate over divisor pairs (d, d2) with d*d2 = abs_p, d <= d2 */
        for (idx_t d = 1; d*d <= abs_p; ++d) {
            if (abs_p % d != 0) continue;
            idx_t d2 = abs_p / d;

            /* Positive product (both positive or both negative) */
            long long sumPos = (long long)d + (long long)d2;
            if (llabs(sumPos) == (long long)s_abs) {
                count += 2;  /* (d, d2) and (-d, -d2) */
            }

            /* Negative product (d, -d2) and (-d, d2) */
            long long diff = (long long)d - (long long)d2;
            if (llabs(diff) == (long long)s_abs) {
                if (d == d2) {
                    count += 1;   /* only one unordered pair (-d, d) */
                } else {
                    count += 2;   /* two distinct unordered pairs */
                }
            }
        }
    }

    return count;
}
