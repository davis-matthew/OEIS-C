/*
 * A306240  Number of ways to write n as x^9 + y^3 + z*(z+1) + w*(w+1), where
 *          x,y,z,w are nonnegative integers with x <= 2 and z <= w.
 * https://oeis.org/A306240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to * n = x^9 + y^3 + z(z+1) + w(w+1),
 *            * where x, y, z, w are non‑negative integers, * x <= 2, z <= w
 * Range:     u8 n <= 15, u16 n <= 3660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm iterates over the small ranges of x (0..2),
 * y (0..cuberoot(n)), and z (0..sqrt(rem1)).
 * For each partial remainder it solves the quadratic
 *   w(w+1) = r
 * by computing the discriminant D = 1 + 4r,
 * taking its integer square root, and checking whether
 * (sqrt(D)-1)/2 is an integer >= z.  All arithmetic
 * uses seq_t, and only casts from seq_t to idx_t
 * occur after we are certain the values fit in 64‑bit. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Pre‑compute 0^9, 1^9, 2^9 */
    static const idx_t pow9[3] = {0, 1, 512};

    for (idx_t xi = 0; xi <= 2; ++xi) {
        idx_t val_x = pow9[xi];
        if (val_x > n)
            continue;
        idx_t rem1 = n - val_x;

        /* y range: 0 .. floor(cuberoot(rem1)) */
        idx_t y = 0;
        for (;; ++y) {
            /* y^3 may overflow if y > 2^21; break early */
            idx_t val_y = y * y * y;
            if (val_y > rem1)
                break;
            idx_t rem2 = rem1 - val_y;

            for (idx_t z = 0; ; ++z) {
                idx_t val_z = z * z + z;
                if (val_z > rem2)
                    break;
                idx_t r = rem2 - val_z;          /* r = w(w+1) */

                /* Solve w(w+1) = r  →  w = (√(1+4r)-1)/2 */
                seq_t D = 1 + (seq_t)4 * r;                 /* D = 1 + 4r */
                seq_t s = floor_sqrt(D);                    /* s = floor(sqrt(D)) */

                if (s * s == D) {                          /* perfect square */
                    /* s must be odd for integer w */
                    if ((s & 1u) != 0) {
                        /* (s-1)/2 fits into idx_t (w <= 2^32-1) */
                        idx_t w = (idx_t)((s - 1) / 2);
                        if (w >= z)
                            ++count;
                    }
                }
            }
        }
    }

    return count;
}
