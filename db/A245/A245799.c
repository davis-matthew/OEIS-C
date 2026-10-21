/*
 * A245799  a(n) = Lucas(3*n) - Fibonacci(n).
 * https://oeis.org/A245799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(3*n) - Fibonacci(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /*  Lucas(0) = 2, Fibonacci(0) = 0  */
    if (n == 0)
        return (seq_t)2;

    /* use the recurrence: Lucas(k) = Fib(k-1) + Fib(k+1) */
    seq_t fn       = fib(n);
    seq_t f3n_minus = fib(3 * n - 1);
    seq_t f3n_plus  = fib(3 * n + 1);

    seq_t lucas3n = f3n_minus + f3n_plus;
    return lucas3n - fn;
}
