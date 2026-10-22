/*
 * A357955  a(n) = 3*binomial(4*n,n) - 20*binomial(3*n,n) +
 *          54*binomial(2*n,n).
 * https://oeis.org/A357955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*binomial(4*n,n) - 20*binomial(3*n,n) +
 *            54*binomial(2*n,n)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 19, i128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t b4 = binomial(4 * n, n);   /* (4n choose n) */
    seq_t b3 = binomial(3 * n, n);   /* (3n choose n) */
    seq_t b2 = binomial(2 * n, n);   /* (2n choose n) */

    /* compute the linear combination */
    return 3 * b4 - 20 * b3 + 54 * b2;
}
