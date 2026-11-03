/*
 * A246554  Concatenation of the n-th Fibonacci number with itself.
 * https://oeis.org/A246554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = fib(n) * (10^digit_len(fib(n)) + 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 794 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Computes the n-th term of the sequence A246554:
 * a(n) = A000045(n) concatenated with itself.
 * This is implemented as a(n) = fib(n) * (10^d + 1),
 * where d is the number of decimal digits of fib(n). */
seq_t a(idx_t n)
{
    seq_t f   = fib(n);            /* nth Fibonacci number */
    int    d  = num_digits(f);      /* number of digits in f (int fits) */
    seq_t pow = pow10(d);          /* 10^d as seq_t */
    seq_t res = f * pow + f;       /* f * 10^d + f  (concatenation) */
    return res;
}
