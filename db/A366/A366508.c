/*
 * A366508  a(n) = Lucas(2*n) + 2*(-1)^n + 1.
 * https://oeis.org/A366508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n) + 2*(-1)^n + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(2*n) = fib(2*n-1) + fib(2*n+1) */

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
    /* compute Lucas(2*n) via Fibonacci numbers */
    seq_t f1 = fib((idx_t)(2*n - 1));
    seq_t f2 = fib((idx_t)(2*n + 1));
    seq_t lucas = f1 + f2;

    /* adjust: +3 for even n, -1 for odd n */
    if ((n & 1ULL) == 0)
        return lucas + (seq_t)3;
    else
        return lucas - (seq_t)1;
}
