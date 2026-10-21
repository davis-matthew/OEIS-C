/*
 * A259285  Expansion of psi(x^2) * f(x, x^7) in powers of x where psi(), f(,)
 *          are Ramanujan theta functions.
 * https://oeis.org/A259285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (u,v) satisfying * 16*n+13 =
 *            (8*u+3)^2 + (8*v+2)^2 * (equivalently, the number of
 *            representations of 16*n+13 as a sum * of a square of the form
 *            (8u+3)^2 and a square of the form (8v+2)^2)
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* m = 16*n + 13 */
    seq_t m = (seq_t)16 * (seq_t)n + (seq_t)13;

    /* maximum |8*u+3| is bounded by sqrt(m) */
    seq_t sqrt_m = floor_sqrt(m);

    /* bound for |u|: |8*u+3| <= sqrt(m)  ==>  |u| <= (sqrt(m)-3)/8
     * we add 1 to be safe for small sqrt_m. */
    idx_t bound_idx = (idx_t)(sqrt_m / (seq_t)8) + (idx_t)1;

    /* signed bound for the for-loop (INT64_MAX is safe anyway) */
    int64_t bound = (bound_idx > (idx_t)INT64_MAX) ? INT64_MAX : (int64_t)bound_idx;

    seq_t count = 0;

    for (int64_t u = -bound; u <= bound; ++u) {
        int64_t x_int = 8 * u + 3;               /* 8*u+3, may be negative */
        seq_t x = (seq_t)(x_int < 0 ? -x_int : x_int); /* absolute value for squaring */

        seq_t x_sq = x * x;
        if (x_sq > m)
            continue;

        seq_t y2 = m - x_sq;
        seq_t y = floor_sqrt(y2);
        if (y * y != y2)
            continue;

        /* y must be congruent to 2 or 6 modulo 8 to correspond to 8*v+2 or 8*v+6 */
        seq_t r = y % (seq_t)8;
        if (r == (seq_t)2 || r == (seq_t)6)
            count += 1;
    }

    return count;
}
