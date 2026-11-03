/*
 * A247338  a(n) = Fibonacci(n) concatenated with Lucas(n).
 * https://oeis.org/A247338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concat_decimal(Fibonacci(n), Lucas(n))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 964 (2026-11-01)
 * Helpers:   fib_pair, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = Fibonacci(n) * 10^{digits(Lucas(n))} + Lucas(n) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fib_pair.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Fibonacci(n) and Fibonacci(n+1) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    /* Lucas(n) = 2*F(n+1) – F(n) */
    seq_t lucas = fn1 * (seq_t)2 - fn;

    /* Number of decimal digits of Lucas(n) */
    int digs = num_digits(lucas);

    /* 10^{digs} as seq_t */
    seq_t pow10d = pow10((idx_t)digs);

    /* Concatenate: F(n) * 10^{digs} + Lucas(n) */
    return fn * pow10d + lucas;
}
