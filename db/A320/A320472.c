/*
 * A320472  a(n) = round(x(n)), where (x(n),y(n)) are defined by the Chirikov
 *          "standard map" y(n) = y(n-1) + sin(x(n-1)), x(n) = x(n-1) + y(n),
 *          with x(0)=y(0)=1.
 * https://oeis.org/A320472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(x(n)), where (x(n),y(n)) are from the Chirikov
 *            standard map: y(n) = y(n-1) + sin(x(n-1)), x(n) = x(n-1) + y(n)
 *            with initial values x(0)=1, y(0)=1. This implements the
 *            definition exactly, using double precision arithmetic for the
 *            intermediate real numbers and rounding to the nearest i...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    double x = 1.0;
    double y = 1.0;

    for (idx_t i = 1; i <= n; ++i) {
        y += sin(x);
        x += y;
    }

    /* cast the rounded value to the return type; the cast is safe because
       the result is non‑negative and fits in a signed long long in the
       intended range of the OEIS data. */
    return (seq_t)llround(x);
}
