/*
 * A357509  a(n) = 2*binomial(3*n,n) - 9*binomial(2*n,n).
 * https://oeis.org/A357509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*binomial(3*n,n) - 9*binomial(2*n,n)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
seq_t a(idx_t n)
{
    seq_t c1 = binomial(3*n, n);
    seq_t c2 = binomial(2*n, n);
    return 2*c1 - 9*c2;
}
