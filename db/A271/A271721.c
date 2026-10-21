/*
 * A271721  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with x
 *          >= y >= z >= 0, x > 0 and w >= z such that (x-y)*(w-z) is a
 *          square.
 * https://oeis.org/A271721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of ordered quadruples (x,y,z,w) with * x >= y
 *            >= z >= 0, x > 0, w >= z and x² + y² + z² + w² = n, * such that
 *            (x−y)*(w−z) is a perfect square. * * The function below
 *            implements this directly by enumerating all * admissible triples
 *            (x,y,z) and solving w² = n − (x²+y²+z²) when * i...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

/* seq_t and idx_t are defined by the build; we use only the
 * operations allowed by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* value below the offset is undefined */

    seq_t count = 0;
    idx_t max_x = _floor_isqrt_u64(n);       /* floor(sqrt(n)) */

    /* Enumerate admissible (x,y,z) triples. */
    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t x2 = x * x;

        for (idx_t y = 0; y <= x; ++y) {
            idx_t y2 = y * y;
            idx_t sum_xy = x2 + y2;
            if (sum_xy > n)
                break;                     /* larger y only increases sum */

            for (idx_t z = 0; z <= y; ++z) {
                idx_t z2 = z * z;
                idx_t sum_xyz = sum_xy + z2;
                if (sum_xyz > n)
                    break;                 /* larger z only increases sum */

                /* Remaining part that must be w². */
                idx_t remaining = n - sum_xyz;
                idx_t w = _floor_isqrt_u64(remaining);
                if (w * w != remaining)
                    continue;                /* no integer w satisfies the sum */

                if (w < z)
                    continue;                /* enforce w >= z */

                /* Check square condition on (x−y)*(w−z). */
                idx_t diff1 = x - y;
                idx_t diff2 = w - z;
                idx_t prod  = diff1 * diff2;
                idx_t sq    = _floor_isqrt_u64(prod);
                if (sq * sq == prod)
                    ++count;
            }
        }
    }

    return count;
}
