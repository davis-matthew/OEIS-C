/*
 * A273432  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with
 *          2*x + y - z a nonnegative cube, where x,y,z,w are nonnegative
 *          integers with y <= z.
 * https://oeis.org/A273432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search:
   Count ordered quadruples (x,y,z,w) of nonnegative integers with y <= z
   such that n = x^2 + y^2 + z^2 + w^2 and 2*x + y - z is a nonnegative cube. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t n_val  = (seq_t)n;

    /* Upper bound for x: x^2 <= n */
    idx_t limit_x = (idx_t)floor_sqrt(n_val);
    for (idx_t x = 0; x <= limit_x; ++x) {
        seq_t sqx = (seq_t)x * x;
        if (sqx > n_val) break;

        /* Upper bound for y: y^2 <= n - x^2 */
        idx_t limit_y = (idx_t)floor_sqrt(n_val - sqx);
        for (idx_t y = 0; y <= limit_y; ++y) {
            seq_t sum_xy = sqx + (seq_t)y * y;

            /* Upper bound for z: z^2 <= n - x^2 - y^2,
               and we enforce y <= z by starting at y. */
            idx_t limit_z = (idx_t)floor_sqrt(n_val - sum_xy);
            for (idx_t z = y; z <= limit_z; ++z) {
                seq_t sum_xyz = sum_xy + (seq_t)z * z;
                if (sum_xyz > n_val) break;   /* safety, should not happen */

                /* 2*x + y - z must be a nonnegative cube */
                int64_t v = 2LL * (int64_t)x + (int64_t)y - (int64_t)z;
                if (v < 0) continue;
                idx_t v_idx = (idx_t)v;
                idx_t m = int_cube_root(v_idx);
                if ((seq_t)m * m * m != (seq_t)v) continue;

                /* Remaining part must be a perfect square for w^2 */
                seq_t rem = n_val - sum_xyz;
                seq_t r = floor_sqrt(rem);
                if (r * r == rem) {
                    total++;          /* w = r is the only valid choice */
                }
            }
        }
    }

    return total;
}
