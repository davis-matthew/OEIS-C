/*
 * A343411  Number of ways to write n as x^3 + [y^3/2] + [z^3/3] + 2^w, where
 *          [.] is the floor function, x,y,z are positive integers, and w is a
 *          nonnegative integer.
 * https://oeis.org/A343411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as x^3 + ⌊y^3/2⌋ + ⌊z^3/3⌋
 *            + 2^w with x, y, z positive integers and w a non‑negative
 *            integer. Computed by brute‑force enumeration
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over powers of two, avoiding overflow */
    idx_t two_w = 1;
    while (two_w <= n) {
        idx_t R = n - two_w;

        /* iterate over x */
        for (idx_t x = 1;; ++x) {
            unsigned __int128 cube_x = (unsigned __int128)x * x * x;
            if (cube_x > R) break;
            idx_t R1 = R - (idx_t)cube_x;

            /* iterate over y */
            for (idx_t y = 1;; ++y) {
                unsigned __int128 cube_y = (unsigned __int128)y * y * y;
                idx_t by = (idx_t)(cube_y / 2);
                if (by > R1) break;
                idx_t R2 = R1 - by;

                /* iterate over z */
                for (idx_t z = 1;; ++z) {
                    unsigned __int128 cube_z = (unsigned __int128)z * z * z;
                    idx_t cz = (idx_t)(cube_z / 3);
                    if (cz > R2) break;
                    if (cz == R2) ++count;
                }
            }
        }

        /* Prepare next power of two, stop if it would overflow or exceed n */
        if (two_w > n / 2) break;
        two_w <<= 1;
    }

    return count;
}
