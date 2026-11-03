/*
 * A381503  Number of rectangles in a Fibonacci(n) X Fibonacci(n+1) grid.
 * https://oeis.org/A381503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fib(n)*(Fib(n)+1)/2) * (Fib(n+1)*(Fib(n+1)+1)/2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 920 (2026-11-01)
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
    /* Get consecutive Fibonacci numbers.  fib() is defined in utils/fib.h
       and returns a seq_t. */
    seq_t fn  = fib(n);       /* Fib(n)   */
    seq_t fn1 = fib(n + 1);   /* Fib(n+1) */

    /* Compute each rectangle count for the corresponding dimension. */
    seq_t part1 = fn * (fn + (seq_t)1);
    part1 /= (seq_t)2;

    seq_t part2 = fn1 * (fn1 + (seq_t)1);
    part2 /= (seq_t)2;

    /* Multiply the two to obtain the total number of rectangles. */
    return part1 * part2;
}
