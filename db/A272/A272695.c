/*
 * A272695  Nearest integer to n*sin(n).
 * https://oeis.org/A272695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = the nearest integer to n*sin(n)
 * Range:     i8 n <= 136, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    double d = (double)n;
    double v = d * sin(d);
    long long r = llround(v);  /* nearest integer to v */
    return (seq_t)r;
}
