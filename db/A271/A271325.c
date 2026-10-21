/*
 * A271325  Number of ordered ways to write n as x^3 + y^2 + z*(3z+1), where
 *          x, y and z are integers with x positive and y nonnegative.
 * https://oeis.org/A271325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts ordered triples (x,y,z) of integers with * x > 0, y
 *            ≥ 0, z ∈ ℤ such that n = x^3 + y^2 + z*(3z+1)
 * Range:     u8 n <= 6, u16 n <= 693, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_cube_root.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* not defined for n = 0; return 0 */
        return 0;

    seq_t count = 0;

    /* maximum x with x^3 ≤ n */
    idx_t x_max = int_cube_root(n);

    for (idx_t x = 1; x <= x_max; ++x)
    {
        seq_t x3        = (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t rem       = (seq_t)n - x3;            /* n - x^3  (rem ≥ 0) */

        /* bound |z| from 3*z^2 + |z| ≤ rem  →  |z| ≤ (√(12*rem+1)+1)/6  */
        seq_t disc      = rem * (seq_t)12 + (seq_t)1;
        seq_t sqrt_disc = floor_sqrt(disc);
        int64_t zmax    = (int64_t)((sqrt_disc + (seq_t)1) / (seq_t)6);

        for (int64_t z = -zmax; z <= zmax; ++z)
        {
            int64_t pen = z * (3 * z + 1);              /* z*(3z+1); always ≥ 0 */
            seq_t  pen_seq = (seq_t)pen;
            if (rem < pen_seq) continue;               /* would make remainder negative */

            seq_t rem2 = rem - pen_seq;               /* y^2 */

            seq_t root   = floor_sqrt(rem2);
            if (root * root == rem2)                  /* y is an integer */
                ++count;
        }
    }

    return count;
}
