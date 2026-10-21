/*
 * A248155  Expansion of (1 + x - x^2)/((1 + x)*(1 + 2*x)).
 * https://oeis.org/A248155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = (-1)^n * (1 + 2^(n-1))
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t t = pow2(n - 1);   /* t = 2^(n-1) */
    t += 1;                  /* t = 1 + 2^(n-1) */

    if (n & 1)               /* if n is odd, negate the result */
        t = -t;

    return t;
}
