/*
 * A395252  a(n) gives positions k >= 1 where the equation
 *          b(b(k)+b(k-1)+b(k-2)) = k fails, for b(k) = floor(k/sqrt(3) +
 *          (5*sqrt(3)-3)/6).
 * https://oeis.org/A395252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that frac((k-2)/sqrt(3)+ (5*sqrt(3)-3)/6) lies in
 *            [0,(2-sqrt(3))/3]
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning k and testing the fractional part condition. */
#include <math.h>

seq_t a(idx_t n)
{
    static double sqrt3 = 0.0, delta = 0.0, beta = 0.0;
    if (sqrt3 == 0.0) {
        sqrt3 = sqrt(3.0);
        delta = (5.0 * sqrt3 - 3.0) / 6.0;
        beta  = (2.0 - sqrt3) / 3.0;
    }

    idx_t count = 0;
    idx_t k     = 1;

    /* small tolerance to guard against rounding errors */
    const double eps = 1e-12;

    for (;;) {
        double val = ((double)k - 2.0) / sqrt3 + delta;
        double frac = val - floor(val);
        if (frac < 0.0) frac = 0.0;             /* avoid -0 */

        if (frac <= beta + eps) {
            count++;
            if (count == n)
                return (seq_t)k;
        }
        k++;
    }
}
