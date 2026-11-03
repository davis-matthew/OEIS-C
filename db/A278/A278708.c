/*
 * A278708  Fibonacci sequence starting 154, 144.
 * https://oeis.org/A278708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 4516 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278708: a(1)=154, a(2)=144;
   a(n) = a(n-1) + a(n-2) for n > 2; equivalent formula:
   a(n) = 154*Fib(n) - 10*Fib(n-1)  (Fib(1)=1, Fib(2)=1) */

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
    if (n == 1) return (seq_t)154;
    if (n == 2) return (seq_t)144;

    seq_t fn  = fib(n);      // Fib(n)
    seq_t fn1 = fib(n-1);    // Fib(n-1)

    return (seq_t)154 * fn - (seq_t)10 * fn1;
}
