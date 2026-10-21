/*
 * A278124  a(n) = 172*2^n - 176.
 * https://oeis.org/A278124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 172*2^n - 176
 * Range:     i8 n <= 0, i16 n <= 7, i32 n <= 23, i64 n <= 55, i128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t t = pow2(n);          /* t = 2^n */
    return (seq_t)172 * t - (seq_t)176;
}
