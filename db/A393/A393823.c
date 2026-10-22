/*
 * A393823  a(n) is the number of unit cubes in the cubic lattice, divided by
 *          24, that have 3 vertices strictly inside and 5 vertices strictly
 *          outside the sphere x^2 + y^2 + z^2 = n.
 * https://oeis.org/A393823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unit cubes in the integer lattice that have
 *            exactly 3 vertices strictly inside the sphere x^2 + y^2 + z^2 =
 *            n and 5 vertices strictly outside, divided by 24
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Integer square root of n (floor). */
    idx_t M = (idx_t)floor_sqrt((seq_t)n);

    /* Range for cube corner coordinates.  Any cube that has an inside
       vertex must lie within [-M-1, M-1] in each dimension. */
    int64_t minC = -(int64_t)(M + 1);
    int64_t maxC = (int64_t)M;

    seq_t total = 0;

    for (int64_t i = minC; i <= maxC; ++i)
    {
        int64_t i0 = i;
        int64_t i1 = i + 1;
        int64_t sx0 = i0 * i0;
        int64_t sx1 = i1 * i1;

        for (int64_t j = minC; j <= maxC; ++j)
        {
            int64_t j0 = j;
            int64_t j1 = j + 1;
            int64_t sy0 = j0 * j0;
            int64_t sy1 = j1 * j1;

            for (int64_t k = minC; k <= maxC; ++k)
            {
                int inside = 0;
                int outside = 0;
                bool valid = true;

                int64_t k0 = k;
                int64_t k1 = k + 1;
                int64_t sz0 = k0 * k0;
                int64_t sz1 = k1 * k1;

                /* Iterate over the 8 vertices of the unit cube. */
                for (int di = 0; di <= 1; ++di)
                {
                    int64_t sx = di ? sx1 : sx0;
                    for (int dj = 0; dj <= 1; ++dj)
                    {
                        int64_t sy = dj ? sy1 : sy0;
                        for (int dk = 0; dk <= 1; ++dk)
                        {
                            int64_t sz = dk ? sz1 : sz0;
                            idx_t dist2 = (idx_t)sx + (idx_t)sy + (idx_t)sz;

                            if (dist2 == n)
                            {
                                valid = false;
                                goto next_cube;
                            }
                            else if (dist2 < n)
                            {
                                ++inside;
                            }
                            else
                            {
                                ++outside;
                            }
                        }
                    }
                }

                if (valid && inside == 3)
                    ++total;

next_cube:
                ;
            }
        }
    }

    /* Each cube is counted 24 times by the above enumeration;
       return the number divided by 24. */
    return total / 24;
}
