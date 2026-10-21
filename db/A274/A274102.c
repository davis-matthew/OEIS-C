/*
 * A274102  a(n) = nearest integer to k*sin(sqrt(k)+j*Pi) where n = 2*k+j, j=0
 *          or 1.
 * https://oeis.org/A274102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest integer to k * sin(sqrt(k) + j * Pi), where n =
 *            2*k + j, j = 0 or 1
 * Range:     i8 n <= 357, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Split n into k and j */
    idx_t k = n >> 1;      /* k = floor(n/2)  */
    int   j = n & 1;       /* j = n mod 2     */

    /* Compute k * sin(sqrt(k)) with long double precision */
    long double sq  = sqrtl((long double)k);
    long double val = (long double)k * sinl(sq);

    /* Apply Pi shift: sin(x + Pi) = -sin(x) */
    if (j) {
        val = -val;
    }

    /* Round to nearest integer */
    long long r;
    if (val >= 0.0L) {
        r = (long long)(val + 0.5L);
    } else {
        r = (long long)(val - 0.5L);
    }

    return (seq_t)r;
}
