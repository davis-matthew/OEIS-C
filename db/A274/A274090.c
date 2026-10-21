/*
 * A274090  a(n) = nearest integer to n^2 * cos(sqrt(n)).
 * https://oeis.org/A274090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest integer to n^2 * cos(sqrt(n))
 * Range:     i8 n <= 11, i16 n <= 221, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* compute n^2 * cos(sqrt(n)) using long double for accuracy */
    long double nd = (long double)n;
    long double prod = nd * nd * cosl(sqrtl(nd));
    /* round to nearest integer */
    long long r = llroundl(prod);
    return (seq_t)r;
}
