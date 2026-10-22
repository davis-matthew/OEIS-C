/*
 * A352356  Number of ways to write 12*n + 5 as 2*x^2 + 5*y^2 + 9*z^2 + x*y*z,
 *          where x, y and z are nonnegative integers.
 * https://oeis.org/A352356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (x,y,z ≥ 0) of 2*x^2 + 5*y^2 + 9*z^2
 *            + x*y*z = 12*n + 5
 * Range:     u8 none, u16 n <= 37, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t target = (seq_t)12 * (seq_t)n + (seq_t)5;
    seq_t count  = 0;

    /* maximum possible x: 2*x^2 ≤ target  →  x ≤ sqrt(target/2) */
    seq_t maxx_sq = target / (seq_t)2;
    idx_t maxx = (idx_t)floor_sqrt(maxx_sq);

    for (idx_t x = 0; x <= maxx; ++x)
    {
        seq_t xseq = (seq_t)x;
        seq_t xx2  = xseq * xseq;
        seq_t term_x = (seq_t)2 * xx2;               /* 2*x^2 */

        if (term_x > target) break;                 /* safety */

        /* remaining part for y: 5*y^2 ≤ target - term_x */
        seq_t rem_for_y = target - term_x;
        seq_t maxy_sq = rem_for_y / (seq_t)5;
        idx_t maxy = (idx_t)floor_sqrt(maxy_sq);

        for (idx_t y = 0; y <= maxy; ++y)
        {
            seq_t yseq = (seq_t)y;
            seq_t yy2  = yseq * yseq;
            seq_t term_xy = (seq_t)5 * yy2;          /* 5*y^2 */
            seq_t total_xy = term_x + term_xy;

            if (total_xy > target) break;            /* safety */

            /* C2 = target - total_xy   (≥ 0) */
            seq_t C2 = target - total_xy;

            /* quadratic: 9*z^2 + (x*y)*z - C2 = 0 */
            seq_t xy = xseq * yseq;
            seq_t D  = xy * xy + (seq_t)36 * C2;     /* discriminant */

            seq_t sqrtD = floor_sqrt(D);
            if (sqrtD * sqrtD != D) continue;        /* not a square */

            if (sqrtD < xy) continue;                /* would give negative z */

            seq_t znum = sqrtD - xy;                 /* = 18*z */
            if (znum % (seq_t)18 != 0) continue;     /* not divisible by 18 */

            /* z = znum / 18 is a non‑negative integer.  Count it. */
            ++count;
        }
    }

    return count;
}
