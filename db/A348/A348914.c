/*
 * A348914  a(n) = Product_{i=n..n+4} A000045(i) mod Sum_{i=n..n+4}
 *          A000045(i).
 * https://oeis.org/A348914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=n}^{n+4} F(i) mod Sum_{i=n}^{n+4} F(i)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 21, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Fast doubling Fibonacci: compute F(n) and F(n+1) into *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;          /* a = F(k), b = F(k+1) where k = n/2 */
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b + b;                     /* 2*F(k+1)          */
    seq_t c = a * (two_b - a);              /* F(2k)              */
    seq_t d = a * a + b * b;                /* F(2k+1)            */
    if ((n & 1) == 0) {                     /* n even             */
        *fn  = c;
        *fn1 = d;
    } else {                                 /* n odd              */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t f0, f1, f2, f3, f4;

    fib_pair(n,     &f0, &f1);   /* f0 = F(n),   f1 = F(n+1)   */
    fib_pair(n + 1, &f1, &f2);   /* f2 = F(n+2)                */
    fib_pair(n + 2, &f2, &f3);   /* f3 = F(n+3)                */
    fib_pair(n + 3, &f3, &f4);   /* f4 = F(n+4)                */

    seq_t sum = f0 + f1 + f2 + f3 + f4;

    seq_t prod = 1;
    prod = (prod * f0) % sum;
    prod = (prod * f1) % sum;
    prod = (prod * f2) % sum;
    prod = (prod * f3) % sum;
    prod = (prod * f4) % sum;

    return prod;
}
