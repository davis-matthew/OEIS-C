/*
 * A282494  Number of ways to write n as x^4 + y^2 + z^2 + w^2 with
 *          y*(y+240*z) a positive square, where x,y,z,w are nonnegative
 *          integers.
 * https://oeis.org/A282494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of ways to write n as
 * x^4 + y^2 + z^2 + w^2 with y > 0 and
 * y*(y+240*z) a perfect square.
 * a(n) is the number of ordered quadruples (x,y,z,w)
 * of non‑negative integers satisfying these conditions. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* avoid overflow in arithmetic on idx_t */
    seq_t count = 0;

    /* x^4 grows fastest, limit x by N^(1/4) */
    for (seq_t x = 0; ; ++x) {
        seq_t x2 = x * x;
        seq_t x4 = x2 * x2;
        if (x4 > N) break;
        seq_t rem1 = N - x4;          /* remaining after x^4 */

        /* y must be positive (to make the product positive) */
        for (seq_t y = 1; ; ++y) {
            seq_t y2 = y * y;
            if (y2 > rem1) break;
            seq_t rem2 = rem1 - y2;    /* remaining after y^2 */

            seq_t maxz = floor_sqrt(rem2);
            for (seq_t z = 0; z <= maxz; ++z) {
                seq_t z2 = z * z;
                seq_t w2 = rem2 - z2;   /* non‑negative by construction */
                seq_t w = floor_sqrt(w2);
                if (w * w != w2) continue;   /* w not integer */

                seq_t v = y * (y + 240 * z); /* y*(y+240*z) */
                seq_t s = floor_sqrt(v);
                if (s * s == v) ++count;      /* perfect square */
            }
        }
    }
    return count;
}
