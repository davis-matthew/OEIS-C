/*
 * A381781  a(n) = k where k*Pi is the solution to sin(x) = 0 obtained using
 *          Newton's method starting from x = n.
 * https://oeis.org/A381781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 98, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = round( x / π ) where x is the limit of Newton's method
 * for sin(x)=0:  x_{k+1} = x_k - tan(x_k),  x_0 = n. */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Using double for Newton iterations. */
    const double pi = acos(-1.0);
    double x = (double)n;
    double prev;
    int iter = 0;

    /* Perform Newton iterations until convergence or a safety limit. */
    do {
        prev = x;
        x -= tan(prev);
        iter++;
    } while (fabs(x - prev) > 1e-12 && iter < 200);

    /* Determine the nearest integer multiple of π. */
    long long k = llround(x / pi);
    return (seq_t)k;
}
