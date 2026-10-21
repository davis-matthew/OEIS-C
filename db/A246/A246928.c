/*
 * A246928  Number of integer solutions to x^2 + 3*y^2 + 3*z^2 = n.
 * https://oeis.org/A246928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of integer solutions to x^2 + 3*y^2 + 3*z^2 = n */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t n_seq = n;

    for (idx_t y = 0;; ++y) {
        seq_t y2 = (seq_t)y * y;
        seq_t yterm = 3 * y2;
        if (yterm > n_seq) break;

        seq_t rem = n_seq - yterm;           /* n - 3*y^2   */
        seq_t zmax_sq = rem / 3;
        idx_t zmax = (idx_t)floor_sqrt(zmax_sq);

        seq_t mult_y = (y == 0) ? (seq_t)1 : (seq_t)2;

        for (idx_t z = 0; z <= zmax; ++z) {
            seq_t z2 = (seq_t)z * z;
            seq_t zterm = 3 * z2;
            seq_t r = rem - zterm;            /* n - 3*(y^2+z^2)  */

            seq_t sqrt_r = floor_sqrt(r);
            if (sqrt_r * sqrt_r == r) {
                seq_t mult_z = (z == 0) ? (seq_t)1 : (seq_t)2;
                seq_t mult = mult_y * mult_z;
                if (r == 0) {
                    count += mult;            /* x = 0 only */
                } else {
                    count += mult * 2;        /* x = ±sqrt(r) */
                }
            }
        }
    }

    return count;
}
