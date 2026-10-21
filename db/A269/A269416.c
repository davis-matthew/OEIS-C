/*
 * A269416  Expansion of 3*(2 - x)/((1 - x)*(1 + x)^2).
 * https://oeis.org/A269416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*((-1)^n*(6*n + 7) + 1)/4 or equivalently: a(n) =
 *            3*(-1)^n*A007494(n+1)
 * Range:     i8 n <= 5, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute the parity-based sign */
    seq_t sign = (n & 1) ? -1 : 1;
    /* term 6*n + 7 in seq_t space */
    seq_t t = 6 * (seq_t)n + 7;
    /* numerator = 3 * (sign*t + 1) */
    seq_t num = 3 * (sign * t + 1);
    /* division by 4 is exact for all n */
    return num / 4;
}
