/*
 * A286812  a(n) = 105 - 2^n.
 * https://oeis.org/A286812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 105 - 2^n
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t two_pow = pow2(n);
    return (seq_t)105 - two_pow;
}
