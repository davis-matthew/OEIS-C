/*
 * A282545  Number of ways to write n as x^4 + y^2 + z^2 + w^2 with y^2 +
 *          64*z^2 + 1024*y*z a square, where x,y,z,w are nonnegative integers
 *          with y > 0.
 * https://oeis.org/A282545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as n = x^4 + y^2 + z^2 +
 *            w^2 where y > 0 and y^2 + 64 z^2 + 1024 y z is a perfect square.
 *            All variables are non‑negative integers
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"    /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* offset is 1 – for n <= 0 the sequence is undefined; return 0 */
    if (n == 0) return 0;

    const seq_t N = (seq_t)n;
    seq_t count = 0;

    /* maximum possible x: x^4 <= N  →  x <= floor_sqrt(floor_sqrt(N)) */
    seq_t sqrtN = floor_sqrt(N);
    seq_t maxX  = floor_sqrt(sqrtN);

    for (seq_t x = 0; x <= maxX; ++x) {
        seq_t x2 = x * x;
        seq_t x4 = x2 * x2;
        if (x4 > N) break;  /* safety guard */

        /* maximum y: y^2 <= N - x^4  →  y <= floor_sqrt( N - x4 )  */
        seq_t maxY = floor_sqrt(N - x4);

        for (seq_t y = 1; y <= maxY; ++y) {   /* y > 0 required */
            seq_t y2  = y * y;
            seq_t xy  = x4 + y2;
            if (xy > N) break;

            /* maximum z: z^2 <= N - xy  →  z <= floor_sqrt( N - xy ) */
            seq_t maxZ = floor_sqrt(N - xy);

            for (seq_t z = 0; z <= maxZ; ++z) {
                seq_t z2 = z * z;
                seq_t xyz = xy + z2;
                if (xyz > N) break;

                /* check the square condition on y and z */
                seq_t cond = y2 + (seq_t)64 * z2 + (seq_t)1024 * y * z;
                seq_t scond = floor_sqrt(cond);
                if (scond * scond != cond) continue;

                /* remaining part must be a square for w */
                seq_t rem = N - xyz;              /* rem = w^2  */
                seq_t sw  = floor_sqrt(rem);
                if (sw * sw == rem)
                    ++count;
            }
        }
    }

    return count;
}
