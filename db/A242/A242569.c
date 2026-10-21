/*
 * A242569  a(n) = n! - 2*n.
 * https://oeis.org/A242569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! - 2*n
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 24,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);       /* compute n! in seq_t */
    seq_t two_n = (seq_t)n * 2;   /* compute 2*n safely in seq_t */
    return f - two_n;             /* n! - 2*n */
}
