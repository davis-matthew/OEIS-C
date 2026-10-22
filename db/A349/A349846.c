/*
 * A349846  Expansion of -(1 - 8*x) / sqrt(1 - 4*x).
 * https://oeis.org/A349846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1; for n>0, a(n) = 8*binomial(2*(n-1), n-1) -
 *            binomial(2*n, n)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)(-1);
    }
    idx_t n1 = n - 1;
    seq_t term1 = binomial(2 * n1, n1);   /* binomial(2*(n-1), n-1) */
    seq_t term2 = binomial(2 * n,     n); /* binomial(2*n, n) */
    return term1 * 8 - term2;
}
