/*
 * A323381  Number of solutions to n = x^2 + 2*y^2 + 5*z^2 + x*z in integers.
 * https://oeis.org/A323381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y,z) to n = x^2 + 2*y^2 +
 *            5*z^2 + x*z. This is a brute‑force search over feasible z and y,
 *            solving the quadratic in x
 * Range:     u8 n <= 15, u16 n <= 3821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                     /* for int64_t */
#include "../utils/floor_sqrt.h"        /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;           /* use seq_t for all arithmetic */
    seq_t four_n = n_seq * 4;         /* 4*n */

    /* Maximum |z| satisfies 19*z^2 <= 4*n. */
    seq_t max_z_sq = four_n / 19;     /* floor division */
    seq_t maxZ = floor_sqrt(max_z_sq);
    if (maxZ > (seq_t)INT64_MAX) maxZ = (seq_t)INT64_MAX;  /* safety */

    /* Loop over all integer z. */
    for (int64_t z = -(int64_t)maxZ; z <= (int64_t)maxZ; ++z)
    {
        idx_t absz = (idx_t)(z < 0 ? -z : z);
        seq_t z_sq = (seq_t)absz * (seq_t)absz;

        seq_t rem_sq = four_n - (seq_t)19 * z_sq;  /* 4*n - 19*z^2 */
        if (rem_sq == 0 && z_sq == 0) {           /* only z=0 case handled */
            ;
        }
        if (rem_sq > four_n) continue;           /* safety, should not happen */

        /* For this z, |y| bounded by 8*y^2 <= rem_sq. */
        seq_t max_y_sq = rem_sq / 8;
        seq_t maxY = floor_sqrt(max_y_sq);
        if (maxY > (seq_t)INT64_MAX) maxY = (seq_t)INT64_MAX;  /* safety */

        for (int64_t y = -(int64_t)maxY; y <= (int64_t)maxY; ++y)
        {
            idx_t absy = (idx_t)(y < 0 ? -y : y);
            seq_t y_sq = (seq_t)absy * (seq_t)absy;

            seq_t D = rem_sq - (seq_t)8 * y_sq;   /* discriminant */
            if (D == 0) {
                /* sqrtD = 0: single root if parity matches */
                seq_t sqrtD = 0;
                if ( ((seq_t)(z & 1)) == (sqrtD & 1) )
                    count += 1;
                continue;
            }

            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD != D) continue;      /* not a perfect square */

            /* Parity check: (-z ± sqrtD) must be even. */
            if ( ((seq_t)(z & 1)) != (sqrtD & 1) ) continue;

            /* Two distinct integer roots. */
            count += 2;
        }
    }
    return count;
}
