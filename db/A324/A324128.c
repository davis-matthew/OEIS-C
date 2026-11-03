/*
 * A324128  a(n) = 2*n*Fibonacci(n) + (-1)^n + 1.
 * https://oeis.org/A324128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*Fib(n) + (-1)^n + 1
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 83, u128 n <= 173,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using the provided fib() helper function from utils/fib.h */

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
    seq_t fn = fib(n);                     /* Fib(n) */
    seq_t res = (seq_t)2 * (seq_t)n * fn;   /* 2 * n * Fib(n) */
    if ((n & 1u) == 0u)                    /* if n is even add 2 */
        res += (seq_t)2;
    return res;
}
