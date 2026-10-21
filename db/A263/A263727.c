/*
 * A263727  Largest square number less than or equal to the n-th Fibonacci
 *          number.
 * https://oeis.org/A263727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(Fibonacci(n)))^2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   fib, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Fibonacci(0) is 0, but the utils/​fib routine has offset 1
       (fib(1)=1).  We therefore handle n==0 specially. */
    if (n == 0)
        return (seq_t)0;

    seq_t f = fib(n);            /* Fibonacci(n) with offset 1 in utils */
    seq_t s = floor_sqrt(f);     /* floor(sqrt(Fibonacci(n))) */
    return s * s;                /* largest square <= Fibonacci(n) */
}
