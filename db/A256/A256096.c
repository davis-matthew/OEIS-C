/*
 * A256096  Expansion of (4+3*x)/(1+3*x).
 * https://oeis.org/A256096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 4; for n >= 1, a(n) = (-1)^n * 3^(n+1)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 38, i128 n <= 79,
 *            big any n
 * Helpers:   pow3_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    seq_t val = pow3_seq(n + 1);   /* 3^(n+1) */
    if (n & 1)
        val = -val;                 /* multiply by (-1)^n */
    return val;
}
