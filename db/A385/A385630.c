/*
 * A385630  a(n) = n! * Prod_{k=1..n} Fibonacci(k).
 * https://oeis.org/A385630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Prod_{k=1..n} Fibonacci(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        result *= i;           /* factorial part */
        result *= fib(i);      /* Fibonacci part */
    }
    return result;
}
