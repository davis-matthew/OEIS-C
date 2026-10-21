/*
 * A278125  a(n) = 225*2^n - 235.
 * https://oeis.org/A278125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 225*2^n - 235
 * Range:     i8 n <= 0, i16 n <= 7, i32 n <= 23, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);          /* 2^n */
    val *= (seq_t)225;            /* 225*2^n */
    val -= (seq_t)235;            /* minus 235 */
    return val;
}
