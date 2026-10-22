/*
 * A316569  a(n) = Jacobi (or Kronecker) symbol (n, 15).
 * https://oeis.org/A316569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Jacobi symbol (n,15)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int8_t tbl[15] = {
        0,  /* n mod 15 = 0  */
        1,  /* 1 */
        1,  /* 2 */
        0,  /* 3 */
        1,  /* 4 */
        0,  /* 5 */
        0,  /* 6 */
        -1, /* 7 */
        1,  /* 8 */
        0,  /* 9 */
        0,  /* 10 */
        -1, /* 11 */
        0,  /* 12 */
        -1, /* 13 */
        -1  /* 14 */
    };
    return (seq_t)tbl[(int)(n % 15)];
}
