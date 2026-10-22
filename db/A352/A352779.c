/*
 * A352779  Powers of -3: a(n) = (-3)^n.
 * https://oeis.org/A352779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-3)^n * Recurrence: a(0) = 1; a(n) = -3 * a(n-1)
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t val = pow3(n);
    if (n & 1) {          /* n odd → negative sign */
        val = -val;
    }
    return val;
}
