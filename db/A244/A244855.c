/*
 * A244855  a(n) = Fibonacci(n)^4-1.
 * https://oeis.org/A244855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)^4 - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 188 (2026-11-01)
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
    seq_t f = fib(n);          /* Fibonacci(n) */
    seq_t f2 = f * f;          /* Fibonacci(n)^2     */
    seq_t f4 = f2 * f2;        /* Fibonacci(n)^4     */
    return f4 - (seq_t)1;      /* subtract 1          */
}
