/*
 * A306637  a(n) = Fibonacci(n) * A128834(n).
 * https://oeis.org/A306637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Fibonacci(n) * A128834(n), where A128834(n) =
 *            {0,1,1,0,-1,-1} repeating with period 6
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-01)
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
    /* pattern for A128834: indexed by n mod 6 */
    static const int8_t pat[6] = {0, 1, 1, 0, -1, -1};
    idx_t m = n % 6;
    seq_t f = fib(n);               /* Fibonacci number F(n) */
    seq_t s = (seq_t)pat[m];        /* sign factor */
    return f * s;
}
