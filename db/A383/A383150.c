/*
 * A383150  a(n) = Sum_{k=0..n} k^3 * (-1)^k * 3^(n-k) * binomial(n,k).
 * https://oeis.org/A383150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n-3) * (-12*n + 9*n^2 - n^3)
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 49, i128 n <= 109,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* polynomial p(n) = -12*n + 9*n^2 - n^3 */
    seq_t p = - (seq_t)12 * nn + (seq_t)9 * nn * nn - nn * nn * nn;

    if (n >= 3) {
        return p * pow2(n - 3);
    } else {
        /* for n < 3 divide by 2^(3-n); the division is exact */
        return p / pow2(3 - n);
    }
}
