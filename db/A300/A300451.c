/*
 * A300451  a(n) = (3*n^2 - 3*n + 8)*2^(n - 3).
 * https://oeis.org/A300451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300451
   Formula: a(n) = (3*n^2 - 3*n + 8) * 2^(n - 3)   (offset 0)
   For n<3 the exponent is negative and the division is exact. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the polynomial part */
    seq_t nn = (seq_t)n;
    seq_t term = 3 * nn * nn - 3 * nn + 8;

    /* Multiply or divide by the appropriate power of two */
    if (n >= 3) {
        return term * pow2(n - 3);
    } else {
        return term / pow2(3 - n);
    }
}
