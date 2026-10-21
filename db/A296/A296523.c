/*
 * A296523  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x > 0, y
 *          >= 0 and 0 <= z <= w such that 64*x^2 + 65*y^2 = q^2 for some
 *          practical number q.
 * https://oeis.org/A296523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_practical_uint64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS definition of a(n):
   a(n) = number of quadruples (x,y,z,w) ∈ ℤ^4 with
   x > 0, y ≥ 0, 0 ≤ z ≤ w and
   n = x² + y² + z² + w²
   such that 64·x² + 65·y² = q² for some practical number q.
   The routine searches all possibilities and counts the valid ones. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/floor_sqrt.h"
#include "../utils/_is_practical_uint64.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* Main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0)                       /* off‑offset; harmless output */
        return 0;

    seq_t total = 0;

    /* Iterate over admissible x > 0 */
    for (idx_t x = 1; (seq_t)x * x <= n; ++x) {
        seq_t x2 = (seq_t)x * x;
        seq_t base64 = (seq_t)64 * x2;               /* 64·x² */

        /* Iterate over admissible y ≥ 0 */
        for (idx_t y = 0; (seq_t)y * y <= n - x2; ++y) {
            seq_t y2 = (seq_t)y * y;
            seq_t remaining = n - x2 - y2;

            /* test the quadratic form for practicality */
            seq_t v = base64 + (seq_t)65 * y2;       /* 64·x² + 65·y² */
            seq_t q = floor_sqrt(v);
            if ((q * q) != v) continue;             /* not a perfect square */

            if (!_is_practical_uint64((uint64_t)q))   /* q not practical */
                continue;

            /* Count (z,w) with 0 ≤ z ≤ w, z² + w² = remaining */
            for (idx_t z = 0; (seq_t)z * z <= remaining; ++z) {
                seq_t z2 = (seq_t)z * z;
                seq_t w2 = remaining - z2;
                seq_t w = floor_sqrt(w2);
                if ((w * w) == w2 && z <= w) {
                    ++total;
                }
            }
        }
    }

    return total;
}
