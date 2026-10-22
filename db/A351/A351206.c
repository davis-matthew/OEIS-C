/*
 * A351206  Least positive integer m such that n = x^4 + (y^4 + z^4 +
 *          7*w^2)/m^4 for some nonnegative integers x,y,z,w with y <= z.
 * https://oeis.org/A351206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by searching the smallest positive integer m such that
   n = x⁴ + (y⁴ + z⁴ + 7*w²)/m⁴ for some non‑negative integers
   x, y, z, w with y ≤ z.  This implementation evaluates the
   condition directly for each candidate m, starting from 1
   and stopping at the first successful value. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Compute v⁴ as seq_t; works safely for all supported sizes. */
static seq_t pow4_seq(seq_t v)
{
    seq_t r = v;
    r *= v;
    r *= v;
    r *= v;
    return r;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;           /* promote to seq_t */
    for (idx_t m = 1; ; ++m)          /* increasing m */
    {
        seq_t m4 = pow4_seq((seq_t)m);      /* m⁴ */

        /* Max x such that x⁴ ≤ n.  Since n ≤ 10⁴, x is tiny. */
        idx_t x0;
        for (x0 = 0; ; ++x0)
            if (pow4_seq((seq_t)x0) > n_seq) break;
        idx_t x_max = x0 - 1;

        for (idx_t x = 0; x <= x_max; ++x)
        {
            seq_t x4   = pow4_seq((seq_t)x);
            seq_t rem  = n_seq - x4;          /* n – x⁴ ≥ 0 */
            seq_t target = m4 * rem;          /* m⁴·(n – x⁴) */

            /* Enumerate y and z with y ≤ z. */
            idx_t y;
            for (y = 0; ; ++y)
            {
                seq_t y4 = pow4_seq((seq_t)y);
                if (y4 > target) break;       /* y too large */

                idx_t z;
                for (z = y; ; ++z)
                {
                    seq_t z4 = pow4_seq((seq_t)z);
                    seq_t sum_yz = y4 + z4;
                    if (sum_yz > target) break;   /* z too large */

                    seq_t remaining = target - sum_yz;
                    if (remaining % (seq_t)7 != 0) continue;

                    seq_t w2 = remaining / (seq_t)7;
                    seq_t w  = floor_sqrt(w2);
                    if (w * w == w2)              /* w integer */
                        return (seq_t)m;          /* found minimal m */
                }
            }
        }
    }
}
