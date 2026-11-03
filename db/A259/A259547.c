/*
 * A259547  a(n) = n^4*Fibonacci(n).
 * https://oeis.org/A259547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*Fib(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 22, u64 n <= 59, u128 n <= 144,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 969 (2026-11-01)
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
    /* Compute Fibonacci(n) */
    seq_t f = fib(n);

    /* Compute n^4 */
    seq_t t = (seq_t)n;
    t *= t;   /* n^2 */
    t *= t;   /* n^4 */

    /* Return n^4 * Fibonacci(n) */
    return t * f;
}
