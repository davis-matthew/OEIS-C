/*
 * A272123  a(n) = Fibonacci(3n) - Fibonacci(2n).
 * https://oeis.org/A272123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(3n) - Fib(2n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-01)
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

seq_t a(idx_t n) {
    idx_t two_n   = n << 1;      /* 2*n */
    idx_t three_n = two_n + n;   /* 3*n */
    seq_t f2 = fib(two_n);
    seq_t f3 = fib(three_n);
    return f3 - f2;
}
