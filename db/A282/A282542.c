/*
 * A282542  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that x + 3*y + 5*z and (at least) one of
 *          y,z,w are squares.
 * https://oeis.org/A282542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9716 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Brute‑force enumeration of all nonnegative integers
   (x,y,z,w) with x²+y²+z²+w² = n.  For each solution count it
   only if
     •  x + 3·y + 5·z is a perfect square, and
     •  at least one of y, z, w is a perfect square.
   The function works for any seq_t (fixed size or arbitrary
   precision) and any idx_t = uint64_t index n. */

#include <stdbool.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;                     /* result counter */
    idx_t rmax = isqrt(n);    /* maximum value for y,z,w,x */

    for (idx_t y = 0; y <= rmax; ++y) {
        idx_t y2 = y * y;
        if (y2 > n) break;

        idx_t zmax = isqrt(n - y2);
        for (idx_t z = 0; z <= zmax; ++z) {
            idx_t z2 = z * z;
            idx_t yz = y2 + z2;
            if (yz > n) break;

            idx_t wmax = isqrt(n - yz);
            for (idx_t w = 0; w <= wmax; ++w) {
                idx_t w2 = w * w;
                idx_t yzw = yz + w2;
                if (yzw > n) break;

                /* x² = n – (y²+z²+w²) */
                idx_t x2 = n - yzw;
                idx_t x   = isqrt(x2);
                if (x * x != x2) continue;          /* x not integer */

                /* linear combination must be a perfect square */
                idx_t linear = x + 3 * y + 5 * z;
                idx_t lroot  = isqrt(linear);
                if (lroot * lroot != linear) continue;

                /* at least one of y, z, w is a perfect square */
                bool ysq = (isqrt(y) * isqrt(y) == y);
                bool zsq = (isqrt(z) * isqrt(z) == z);
                bool wsq = (isqrt(w) * isqrt(w) == w);
                if (!ysq && !zsq && !wsq) continue;

                ++count;          /* found a valid representation */
            }
        }
    }

    return count;
}
