/*
 * A382843  Length of the long leg in the unique primitive Pythagorean triple
 *          (x,y,z) such that (x-y+z)/2 = A000045(n) and its long leg and
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A382843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*Fibonacci(n)*(Fibonacci(n) - 1)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 46, i128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the nth Fibonacci number using the provided helper. */
    seq_t f = fib(n);

    /* a(n) = 2*f*(f-1). The calculation fits in seq_t for all n for which
       the result itself fits in seq_t (as required by the test harness).  */
    return (seq_t)2 * f * (f - 1);
}
