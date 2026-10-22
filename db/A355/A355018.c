/*
 * A355018  Partial sums of F(1) - L(1) + F(2) - L(2) + F(3) - L(3) + ...,
 *          where F = A000045 and L = A000032.
 * https://oeis.org/A355018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 - F((n+2)/2) if n is even * a(n) = 2 - 2*F((n+3)/2) if
 *            n is odd * where F(k) is the Fibonacci number with F(0)=0,
 *            F(1)=1. * The sequence also satisfies
 *            a(n)=a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5) for n≥5, * but the
 *            closed form above is more efficient for any n
 * Range:     i8 n <= 18, i16 n <= 40, i32 n <= 86, i64 n <= 178,
 *            i128 n <= 364, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h" // provides seq_t fib(idx_t n);

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0) {                   /* even n */
        seq_t f = fib((n + 2) >> 1);             /* F((n+2)/2) */
        return (seq_t)2 - f;
    } else {                                     /* odd n */
        seq_t f = fib((n + 3) >> 1);             /* F((n+3)/2) */
        seq_t twice = f + f;                     /* 2*F((n+3)/2) */
        return (seq_t)2 - twice;
    }
}
