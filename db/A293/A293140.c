/*
 * A293140  E.g.f.: Product_{m>0} (1-x^m).
 * https://oeis.org/A293140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * coeff where coeff = 0 unless n is a generalized
 *            pentagonal number * m(3m-1)/2 for some integer m. If so coeff =
 *            (-1)^abs(m)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 14, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute D = 1 + 24*n. */
    idx_t D = (idx_t)1 + (idx_t)24 * n;

    /* Integer square root of D. */
    idx_t s = 0;
    while ((s + 1) <= D && (s + 1) * (s + 1) <= D)
        ++s;

    /* If D is not a perfect square, a(n) = 0. */
    if (s * s != D)
        return (seq_t)0;

    /* Determine abs(m). For a perfect square, either (1+s) or (s-1) is divisible by 6. */
    idx_t abs_m;
    if ((1 + s) % 6 == 0)
        abs_m = (1 + s) / 6;
    else
        abs_m = (s - 1) / 6;

    /* Sign factor: +1 if abs_m even, -1 if abs_m odd. */
    seq_t coeff = ((abs_m & 1) == 0) ? (seq_t)1 : (seq_t)-1;

    /* Return factorial times coeff. */
    return factorial(n) * coeff;
}
