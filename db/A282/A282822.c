/*
 * A282822  a(n) = (n - 4)*n! for n>=0.
 * https://oeis.org/A282822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n - 4)*n!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    seq_t fact = factorial(n);
    return fact * (seq_n - 4);
}
