/*
 * A372225  a(n) = Fibonacci(n+1)*Fibonacci(2n).
 * https://oeis.org/A372225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1) * Fibonacci(2n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = Fibonacci(2) * Fibonacci(2) = 1 */
    if (n == 0) return 0;            /* sentinel for out‑of‑range calls */

    seq_t f1 = fib(n + 1);            /* Fibonacci(n+1) */
    seq_t f2 = fib(2 * n);            /* Fibonacci(2n)   */
    return f1 * f2;                   /* product as required */
}
