/*
 * A354044  a(n) = 2*(-i)^n*(n*sin(c*(n+1)) - i*sin(-c*n))/sqrt(5) where c =
 *          arccos(i/2).
 * https://oeis.org/A354044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + n * Fibonacci(n+1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = F(n) + n*F(n+1)  (F(0)=0, F(1)=1) */

#include <stddef.h>  /* for NULL, size_t, etc. */

seq_t a(idx_t n)
{
    /* Iteratively compute Fibonacci(n) and Fibonacci(n+1). */
    seq_t fib_n   = 0;  /* F(0) */
    seq_t fib_np1 = 1;  /* F(1) */

    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = fib_n + fib_np1;  /* F(i+2) */
        fib_n   = fib_np1;            /* shift */
        fib_np1 = tmp;
    }

    /* Now fib_n = F(n), fib_np1 = F(n+1) */
    return fib_n + (seq_t)n * fib_np1;
}
