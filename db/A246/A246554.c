/*
 * A246554  Concatenation of the n-th Fibonacci number with itself.
 * https://oeis.org/A246554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = fib(n) * (10^digit_len(fib(n)) + 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   digit_len, fib, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Computes the n-th term of the sequence A246554:
 * a(n) = A000045(n) concatenated with itself.
 * This is implemented as a(n) = fib(n) * (10^d + 1),
 * where d is the number of decimal digits of fib(n). */
seq_t a(idx_t n)
{
    seq_t f   = fib(n);            /* nth Fibonacci number */
    int    d  = digit_len(f);      /* number of digits in f (int fits) */
    seq_t pow = pow10(d);          /* 10^d as seq_t */
    seq_t res = f * pow + f;       /* f * 10^d + f  (concatenation) */
    return res;
}
