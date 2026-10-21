/*
 * A241290  The integer-valued quartic beginning: 0, 9, 0, 9, 7.
 * https://oeis.org/A241290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -(65/24)n^4 + (89/4)n^3 - (1363/24)n^2 + (185/4)n
 *            Equivalent integer-valued form: a(n) = -65*C(n,4) + 36*C(n,3) -
 *            18*C(n,2) + 9*C(n,1) where C is the binomial coefficient
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute binomial coefficients safely using the provided utility. */
    seq_t p1 = binomial(n, 1);   /* = n */
    seq_t p2 = binomial(n, 2);
    seq_t p3 = binomial(n, 3);
    seq_t p4 = binomial(n, 4);

    /* Apply the integer polynomial.  All intermediate operations use the
       seq_t type, so we avoid overflow for all values that fit in the
       target type. */
    seq_t result = -65 * p4 + 36 * p3 - 18 * p2 + 9 * p1;
    return result;
}
