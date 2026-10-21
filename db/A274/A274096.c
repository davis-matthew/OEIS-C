/*
 * A274096  a(n) = nearest integer to k*sin(sqrt(k)+j*Pi/2) where n = 2*k+j,
 *          0<=j<=1.
 * https://oeis.org/A274096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest integer of k*sin(sqrt(k)+j*Pi/2) where n = 2*k +
 *            j (j = 0 or 1)
 * Range:     i8 n <= 292, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 1;            /* k = floor(n/2) */
    double root = sqrt((double)k);
    double prod;
    if ((n & 1) == 0) {          /* even n: sin(sqrt(k)) */
        prod = k * sin(root);
    } else {                    /* odd n: cos(sqrt(k)) */
        prod = k * cos(root);
    }
    return (seq_t)llround(prod); /* nearest integer */
}
