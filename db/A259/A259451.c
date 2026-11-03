/*
 * A259451  a(n) = n^2*Fibonacci(n).
 * https://oeis.org/A259451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * Fibonacci(n)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 33, u64 n <= 75, u128 n <= 164,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 989 (2026-11-01)
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
    seq_t sn = (seq_t)n;          /* convert index to sequence type */
    return (sn * sn) * fib(n);    /* n^2 * Fib(n) */
}
