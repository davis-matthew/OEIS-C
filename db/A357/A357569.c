/*
 * A357569  a(n) = binomial(3*n,n)^2 - 27*binomial(2*n,n).
 * https://oeis.org/A357569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = binomial(3*n,n)^2 - 27*binomial(2*n,n)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m3 = n * 3;
    idx_t m2 = n * 2;
    seq_t b1 = binomial(m3, n);
    seq_t b2 = binomial(m2, n);
    return b1 * b1 - (seq_t)27 * b2;
}
