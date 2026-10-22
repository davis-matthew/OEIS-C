/*
 * A383151  a(n) = Sum_{k=0..n} k^4 * (-1)^k * 3^(n-k) * binomial(n,k).
 * https://oeis.org/A383151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^(n-4) * (-66*n + 75*n^2 - 18*n^3 + n^4)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 19, i64 n <= 45, i128 n <= 104,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* cast the unsigned index to the arithmetic type */
    seq_t sn  = (seq_t)n;
    seq_t sn2 = sn * sn;            /* n^2  */
    seq_t sn3 = sn2 * sn;           /* n^3  */
    seq_t sn4 = sn3 * sn;           /* n^4  */

    /* the polynomial part of the formula */
    seq_t poly = sn4 - (seq_t)18 * sn3  + (seq_t)75 * sn2  - (seq_t)66 * sn;

    /* multiply or divide by the power of two depending on n */
    if (n >= 4) {
        /* n-4 is non‑negative */
        return poly * pow2(n - 4);
    } else {
        /* divide by 2^(4-n); the division is exact for all n */
        return poly / pow2(4 - n);
    }
}
