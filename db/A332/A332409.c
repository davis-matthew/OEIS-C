/*
 * A332409  a(n) = n!! mod Fibonacci(n).
 * https://oeis.org/A332409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n!! mod Fibonacci(n)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
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
    /* Fibonacci number F(n) */
    seq_t fn = fib(n);

    /* If F(n) == 1, the result is always 0. */
    if (fn == (seq_t)1)
        return (seq_t)0;

    seq_t mod = fn;
    seq_t res = 1 % mod;

    for (idx_t k = n; k > 0; ) {
        res = (res * (seq_t)k) % mod;
        if (k <= 2)
            break;        /* avoid unsigned underflow when k == 1 or 2 */
        k -= 2;
    }
    return res;
}
