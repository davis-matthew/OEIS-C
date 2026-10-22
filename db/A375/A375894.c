/*
 * A375894  a(n) = 8^n*sin (nB - nC)/sqrt(15), where A, B, C are,
 *          respectively, the angles opposite sides BC, CA, AB in a triangle
 *          ABC having sidelengths |BC| = 2, |CA| = 3, |AB| = 4; ABC is the
 *          smallest integer-sided scalene triangle.
 * https://oeis.org/A375894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 14*a(n-1) - 64*a(n-2), with a(0) = 0, a(1) = -1
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)                return (seq_t)0;
    if (n == 1)                return (seq_t)(-1);

    seq_t a0 = (seq_t)0;   /* a(n-2) */
    seq_t a1 = (seq_t)(-1);/* a(n-1) */
    seq_t ai;

    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 * (seq_t)14;
        ai -= a0 * (seq_t)64;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
