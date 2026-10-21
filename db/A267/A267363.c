/*
 * A267363  a(0) = 0; a(odd n) = 2n+1; a(even n) = a(n/2 - a(n-1)).
 * https://oeis.org/A267363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; a(odd n) = 2*n + 1; a(even n) = a(n/2 - a(n-1))
 * Range:     i8 n <= 25, i16 n <= 409, i32 n <= 9585, i64 n <= 9585,
 *            i128 n <= 9585, big any n
 * Helpers:   _A
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_A.h"

/* public entry point with unsigned idx_t argument */
seq_t a(idx_t n)
{
    return _A((int64_t)n);
}
