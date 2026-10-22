/*
 * A383152  a(n) = Sum_{k=0..n} k^5 * (-1)^k * 3^(n-k) * binomial(n,k).
 * https://oeis.org/A383152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} k^5 * (-1)^k * 3^(n-k) * binomial(n,k) =
 *            2^(n-5) * (-480*n + 690*n^2 - 255*n^3 + 30*n^4 - n^5)
 * Range:     i8 n <= 1, i16 n <= 8, i32 n <= 19, i64 n <= 42, i128 n <= 99,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;

    seq_t poly = -(seq_t)480 * nn
               + (seq_t)690 * n2
               - (seq_t)255 * n3
               + (seq_t)30  * n4
               - n5;                 /* poly = -480*n + 690*n^2 - 255*n^3 + 30*n^4 - n^5 */

    if (n >= 5) {
        seq_t mul = pow2(n - 5);   /* 2^(n-5) */
        return poly * mul;
    } else {
        seq_t div = pow2(5 - n);   /* 2^(5-n) */
        return poly / div;
    }
}
