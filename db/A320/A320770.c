/*
 * A320770  a(n) = (-1)^floor(n/4) * 2^floor(n/2).
 * https://oeis.org/A320770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor(n/4) * 2^floor(n/2)
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* floor(n/2) gives the exponent for the power of two */
    idx_t exp = n / 2;
    seq_t val   = pow2(exp);          /* 2^exp */

    /* Apply the sign: (-1)^floor(n/4) */
    if ((n / 4) & 1U)                 /* if floor(n/4) is odd */
        val = -val;

    return val;
}
