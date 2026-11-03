/*
 * A317408  a(n) = n * Fibonacci(2n).
 * https://oeis.org/A317408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*Fibonacci(2n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 983 (2026-11-01)
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
    seq_t fn = fib(2 * n);
    return (seq_t)n * fn;
}
