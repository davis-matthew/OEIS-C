/*
 * A262824  Number of ordered ways to write n as w^2 + x^3 + 2*y^3 + 3*z^3,
 *          where w, x, y and z are nonnegative integers.
 * https://oeis.org/A262824
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

/* Count representations n = w^2 + x^3 + 2*y^3 + 3*z^3
   (w,x,y,z are non‑negative integers, order matters) */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t seq_n = (seq_t)n;
    seq_t count = 0;

    for (idx_t z = 0;; ++z) {
        seq_t zCube = (seq_t)z * z * z;
        seq_t thrZ = 3 * zCube;
        if (thrZ > seq_n) break;
        seq_t rem1 = seq_n - thrZ;

        for (idx_t y = 0;; ++y) {
            seq_t yCube = (seq_t)y * y * y;
            seq_t thrY = 2 * yCube;
            if (thrY > rem1) break;
            seq_t rem2 = rem1 - thrY;

            for (idx_t x = 0;; ++x) {
                seq_t xCube = (seq_t)x * x * x;
                if (xCube > rem2) break;
                seq_t rem3 = rem2 - xCube;

                seq_t w = floor_sqrt(rem3);        // smallest integer >= sqrt(rem3)
                if (w * w == rem3) count++;         // perfect square?
            }
        }
    }
    return count;
}
