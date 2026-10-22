/*
 * A330033  a(n) = Kronecker(n, 5) * (-1)^floor(n/5).
 * https://oeis.org/A330033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Kronecker(n, 5) * (-1)^floor(n/5)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Values are periodic with period 10: indices modulo 10 */
    /* table[i] = a(i) for i = 0..9 */
    static const seq_t table[10] = {
        0,  /* 0 */
        1,  /* 1 */
        -1, /* 2 */
        -1, /* 3 */
        1,  /* 4 */
        0,  /* 5 */
        -1, /* 6 */
        1,  /* 7 */
        1,  /* 8 */
        -1  /* 9 */
    };
    /* idx_t is unsigned, modulo operation is safe */
    idx_t r = n % 10;
    return table[r];
}
