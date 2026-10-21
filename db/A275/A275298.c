/*
 * A275298  Number of ordered ways to write n as w^3 + x^2 + y^2 + z^2 with x
 *          - w a square, where x,y,z,w are nonnegative integers with y <= z >
 *          w.
 * https://oeis.org/A275298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered solutions for n = w^3 + x^2 + y^2 + z^2
 *            with x - w a perfect square, y <= z > w. Computed by brute‑force
 *            search
 * Range:     u8 n <= 31, u16 n <= 690, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   floor_cube_root, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t total = 0;

    /* maximum w such that w^3 <= n */
    idx_t wmax = (idx_t)floor_cube_root((seq_t)n);

    for (idx_t w = 0; w <= wmax; ++w)
    {
        seq_t w3 = (seq_t)w * (seq_t)w * (seq_t)w;
        seq_t M   = (seq_t)n - w3;      /* remaining after w^3 */

        /* z must satisfy z > w and z^2 <= M */
        idx_t zmin = w + 1;
        idx_t zmax = (idx_t)floor_sqrt(M);
        if (zmin > zmax)
            continue;

        for (idx_t z = zmin; z <= zmax; ++z)
        {
            seq_t z2 = (seq_t)z * (seq_t)z;
            if (z2 > M)
                continue;

            seq_t M2 = M - z2;         /* remaining for x^2 + y^2 */

            /* iterate over possible s where x = w + s^2 */
            idx_t smax = (idx_t)floor_sqrt(M2);
            for (idx_t s = 0; s <= smax; ++s)
            {
                seq_t s2 = (seq_t)s * (seq_t)s;
                seq_t x  = (seq_t)w + s2;
                seq_t x2 = x * x;
                if (x2 > M2)
                    continue;

                seq_t rem = M2 - x2;        /* y^2 must equal rem */
                seq_t y   = floor_sqrt(rem);
                if (y * y != rem)
                    continue;

                if (y > (seq_t)z)          /* enforce y <= z */
                    continue;

                /* (w, x, y, z) is a valid ordered solution */
                ++total;
            }
        }
    }

    return total;
}
