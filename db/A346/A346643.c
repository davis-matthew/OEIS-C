/*
 * A346643  Number of ways to write n as w^2 + 2*x^2 + 3*y^4 + 4*z^4, where
 *          w,x,y,z are nonnegative integers.
 * https://oeis.org/A346643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Representation count of n as w^2 + 2*x^2 + 3*y^4 + 4*z^4 : a(n) */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Count the number of quadruples (w,x,y,z) of non‑negative integers
       satisfying n = w^2 + 2*x^2 + 3*y^4 + 4*z^4.  All calculations are
       performed in seq_t to avoid overflow. */
    idx_t count = 0;
    seq_t n_val = (seq_t)n;

    /* Outer loop over z: 4*z^4 <= n  */
    idx_t max_z = (idx_t)floor_sqrt(n_val / 4);
    for (idx_t z = 0; z <= max_z; ++z) {
        seq_t z4 = (seq_t)z * z * z * z;          /* z^4            */
        seq_t val_z = (seq_t)4 * z4;              /* 4*z^4           */
        if (val_z > n_val) break;
        seq_t rem1 = n_val - val_z;               /* n - 4*z^4       */

        /* Loop over y: 3*y^4 <= rem1  */
        seq_t rem1_div3 = rem1 / 3;
        idx_t max_y = (idx_t)floor_sqrt(rem1_div3);
        for (idx_t y = 0; y <= max_y; ++y) {
            seq_t y4 = (seq_t)y * y * y * y;      /* y^4            */
            seq_t val_y = (seq_t)3 * y4;          /* 3*y^4           */
            if (val_y > rem1) break;
            seq_t rem2 = rem1 - val_y;            /* remaining after y */

            /* Loop over x: 2*x^2 <= rem2  */
            seq_t rem2_div2 = rem2 / 2;
            idx_t max_x = (idx_t)floor_sqrt(rem2_div2);
            for (idx_t x = 0; x <= max_x; ++x) {
                seq_t x2 = (seq_t)x * x;          /* x^2            */
                seq_t val_x = (seq_t)2 * x2;      /* 2*x^2           */
                if (val_x > rem2) break;
                seq_t rem3 = rem2 - val_x;        /* remaining after x */

                /* Now find w such that w^2 == rem3  */
                seq_t w = floor_sqrt(rem3);
                if (w * w == rem3) {
                    ++count;
                }
            }
        }
    }
    return (seq_t)count;
}
