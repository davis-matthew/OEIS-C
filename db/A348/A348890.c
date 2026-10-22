/*
 * A348890  Number of ways to write n as 5*w^4 + x^4 + y^2 + z^2, where
 *          w,x,y,z are nonnegative integers with y <= z.
 * https://oeis.org/A348890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as 5*w^4 + x^4 + y^2 + z^2 with
 *            w,x,y,z nonnegative integers and y <= z. The function enumerates
 *            all solutions directly
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* seq_t floor_sqrt(seq_t) */

/* count the number of representations of n */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nval  = (seq_t)n;

    for (idx_t w = 0;; ++w)
    {
        /* compute w^4 */
        seq_t w4 = (seq_t)w;
        w4 *= w;
        w4 *= w;
        w4 *= w;

        seq_t term1 = w4 * 5;
        if (term1 > nval) break;

        for (idx_t x = 0;; ++x)
        {
            /* compute x^4 */
            seq_t x4 = (seq_t)x;
            x4 *= x;
            x4 *= x;
            x4 *= x;

            seq_t term2 = term1 + x4;
            if (term2 > nval) break;

            seq_t rem = nval - term2;                 /* remaining value for y^2 + z^2 */

            for (idx_t y = 0;; ++y)
            {
                seq_t y2 = (seq_t)y * y;
                if (y2 > rem) break;

                seq_t zsq = rem - y2;                /* z^2 must equal this */
                seq_t z   = floor_sqrt(zsq);         /* integer square root */

                if (z * z == zsq && z >= y) {
                    count += 1;
                }
            }
        }
    }
    return count;
}
