/*
 * A350021  Number of ways to write n as w^4 + x^2 + y^2 + z^2 with x - y a
 *          power of two (including 2^0 = 1).
 * https://oeis.org/A350021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (w,x,y,z) of non‑negative integers
 *            satisfying w^4 + x^2 + y^2 + z^2 = n and x - y = 2^k for some k
 *            ≥ 0
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* Loop over w such that w^4 ≤ n.  w fits in idx_t. */
    for (idx_t w = 0; ; ++w)
    {
        seq_t w_u = (seq_t)w;
        seq_t w2  = w_u * w_u;
        seq_t w4  = w2 * w2;
        if (w4 > n) break;            /* w^4 would exceed n */

        seq_t rest_n = n - w4;        /* remainder to be expressed as x^2 + y^2 + z^2 */

        seq_t sqrt_rest = floor_sqrt(rest_n);

        /* y ranges from 0 to sqrt(rest_n). */
        for (idx_t y = 0; y <= sqrt_rest; ++y)
        {
            seq_t y_u = (seq_t)y;
            seq_t y2  = y_u * y_u;

            /* d = x - y must be a positive power of two ≤ sqrt(rest_n). */
            seq_t d = 1;
            for ( ; d <= sqrt_rest; d <<= 1)
            {
                seq_t x = y_u + d;
                if (x > sqrt_rest) break;   /* x would exceed sqrt(rest_n) */

                seq_t x2 = x * x;
                seq_t sum_yx = y2 + x2;
                if (sum_yx > rest_n) continue;   /* impossible to fit z^2 */

                seq_t r = rest_n - sum_yx;   /* r = z^2 */
                seq_t z = floor_sqrt(r);
                if (z * z == r)              /* z is integer */
                    ++count;
            }
        }
    }

    return count;
}
