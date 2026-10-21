/*
 * A245302  Curvature (truncated to integer) of a circle inscribed between a
 *          unit circle and a vertex of a circumscribed regular n-gon.
 * https://oeis.org/A245302
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/sin(Pi/(2*n))^2 - 1)
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined – return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Compute the value using double precision. */
    const double theta = acos(-1.0) / (2.0 * (double)n); /* Pi/(2*n) */
    const double s     = sin(theta);
    const double val   = 1.0 / (s * s) - 1.0;          /* cot^2(theta) */
    /* Guard against negligible floating‑point errors. */
    const unsigned long long result = (unsigned long long)floor(val + 1e-12);

    return (seq_t)result;
}
