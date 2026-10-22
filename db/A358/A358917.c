/*
 * A358917  a(n) = Fibonacci(n+1)^4 - Fibonacci(n-1)^4.
 * https://oeis.org/A358917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1)^4 - Fibonacci(n-1)^4 with Fibonacci(0)=0,
 *            Fibonacci(1)=1. The sequence is indexed from n=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

static inline seq_t pow4(seq_t x)
{
    seq_t sq = x * x;           /* x^2   */
    return sq * sq;             /* x^4   */
}

seq_t a(idx_t n)
{
    /* a(0)=0 by definition (F(1)^4 - F(-1)^4 = 0). */
    if (n == 0) return (seq_t)0;

    /* a(1)=1 is a special case because the general formula
       uses Fibonacci(-1) which is not defined by fib().  */
    if (n == 1) return (seq_t)1;

    /* For n >= 2, Fibonacci(n-1) exists normally. */
    seq_t f_n_plus1 = fib(n + 1);  /* Fibonacci(n+1)   */
    seq_t f_n_minus1 = fib(n - 1); /* Fibonacci(n-1)   */

    return pow4(f_n_plus1) - pow4(f_n_minus1);
}
