/*
 * A275738  Number of ordered ways to write n as w^2 + x^2*(1+y^2+z^2), where
 *          w,x,y,z are nonnegative integers with x > 0, y <= z and y == z
 *          (mod 2).
 * https://oeis.org/A275738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered quadruples (w,x,y,z) of non‑negative *
 *            integers such that * n = w^2 + x^2 * (1 + y^2 + z^2) * with x >
 *            0, y ≤ z and y ≡ z (mod 2)
 * Range:     u8 n <= 15, u16 n <= 769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Return the number of representations of n. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t ns = (seq_t)n;               /* Convert once for inner use. */

    /* Loop over x.  x must be > 0. */
    for (idx_t x = 1; (seq_t)x * (seq_t)x <= ns; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        /* Loop over y (no constraint on parity yet). */
        for (idx_t y = 0; (seq_t)y * (seq_t)y <= ns; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            /* Loop over z ≥ y, with y ≡ z (mod 2). */
            for (idx_t z = y; ; ++z) {
                /* Ensure y ≤ z and equal parity. */
                if (((z & 1u) != (y & 1u))) continue;   /* parity mismatch */
                seq_t z2 = (seq_t)z * (seq_t)z;
                seq_t base = 1 + y2 + z2;               /* 1 + y^2 + z^2   */
                seq_t t = x2 * base;                    /* x^2 * base      */
                if (t > ns) break;                      /* further z only enlarge t */
                seq_t rest = ns - t;                    /* w^2 must equal this */
                seq_t w = floor_sqrt(rest);             /* integer sqrt    */
                if (w * w == rest)                       /* perfect square  */
                    ++count;                            /* found a valid tuple */
            }
        }
    }
    return count;
}
