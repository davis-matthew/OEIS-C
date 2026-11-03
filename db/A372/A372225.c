/*
 * A372225  a(n) = Fibonacci(n+1)*Fibonacci(2n).
 * https://oeis.org/A372225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1) * Fibonacci(2n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = Fibonacci(2) * Fibonacci(2) = 1 */
    if (n == 0) return 0;            /* sentinel for out‑of‑range calls */

    seq_t f1 = fib(n + 1);            /* Fibonacci(n+1) */
    seq_t f2 = fib(2 * n);            /* Fibonacci(2n)   */
    return f1 * f2;                   /* product as required */
}
