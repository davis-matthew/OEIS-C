/*
 * A274101  a(n) = nearest integer to k*sin(sqrt(k)+j*Pi/2) where n = 4*k+j,
 *          0<=j<4.
 * https://oeis.org/A274101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest integer to k*sin(sqrt(k)+j*Pi/2) where n = 4*k+j,
 *            0<=j<4
 * Range:     i8 n <= 584, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 2;          /* floor(n/4) */
    idx_t j = n & 3;           /* n mod 4 */

    /* precomputed constants */
    static const double half_pi = 1.5707963267948966; /* Pi/2 */
    double angle = sqrt((double)k) + (double)j * half_pi;
    double s = sin(angle);
    double val = (double)k * s;

    return (seq_t)llround(val); /* nearest integer */
}
