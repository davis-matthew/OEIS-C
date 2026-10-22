/*
 * A346513  a(n) = Fibonacci(n+1)^3 - Fibonacci(n)^3.
 * https://oeis.org/A346513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+1)^3 - Fibonacci(n)^3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Compute (F(n), F(n+1)) using fast doubling; result in fn, fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);              /* a = F(k), b = F(k+1), k = n/2 */

    /* Compute F(2k)  = a * (2*b - a) */
    seq_t two_b = b + b;
    seq_t diff  = 0;
    if (a <= two_b) diff = two_b - a;      /* 2*b - a is always non‑negative */
    seq_t c = a * diff;                    /* F(2k)   */

    /* Compute F(2k+1) = a^2 + b^2 */
    seq_t d = a * a + b * b;                /* F(2k+1) */

    if ((n & 1) == 0) {                    /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                               /* n odd  */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1); /* fn  = F(n),  fn1 = F(n+1) */

    seq_t fn_sq   = fn  * fn;        /* F(n)^2   */
    seq_t fn1_sq  = fn1 * fn1;       /* F(n+1)^2 */

    seq_t fn_cube  = fn_sq  * fn;     /* F(n)^3   */
    seq_t fn1_cube = fn1_sq * fn1;    /* F(n+1)^3 */

    return fn1_cube - fn_cube;        /* F(n+1)^3 - F(n)^3 */
}
