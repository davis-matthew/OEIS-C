/*
 * A247337  a(n) = Lucas(n) concatenated with Fibonacci(n).
 * https://oeis.org/A247337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concat(Lucas(n), Fibonacci(n))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 92,
 *            big any n
 * Helpers:   digit_len, fib_pair, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lucas(n) = 2*Fibonacci(n+1) – Fibonacci(n) */
/* The code uses the provided helpers fib_pair, pow10 and digit_len. */

#include "../utils/fib_pair.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) and Fibonacci(n+1) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);       /* fn = F(n), fn1 = F(n+1) */

    /* Lucas(n) = 2*F(n+1) – F(n) */
    seq_t lucas = fn1 + fn1;      /* 2*F(n+1) */
    lucas -= fn;                  /* subtract F(n) */

    /* Number of decimal digits in F(n) */
    int digits = digit_len(fn);
    seq_t pow10d = pow10((idx_t)digits);

    /* Concatenate lucas and fn: lucas * 10^digits + fn */
    return lucas * pow10d + fn;
}
