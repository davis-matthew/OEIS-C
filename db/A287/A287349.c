/*
 * A287349  Number of matchings in the n-gear graph.
 * https://oeis.org/A287349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n) + n*Fibonacci(2*n) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Fast doubling method: computes F(n) and F(n+1) into *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    idx_t k = n >> 1;
    seq_t f_k, f_k1;
    fib_pair(k, &f_k, &f_k1);               /* f_k = F(k),  f_k1 = F(k+1) */

    seq_t two_fk1 = f_k1 + f_k1;             /* 2*F(k+1)          */
    seq_t f_2k   = f_k * (two_fk1 - f_k);    /* F(2k)   */
    seq_t f_2k1  = f_k * f_k + f_k1 * f_k1;  /* F(2k+1) */

    if ((n & 1) == 0) {                      /* n even */
        *fn  = f_2k;
        *fn1 = f_2k1;
    } else {                                 /* n odd */
        *fn  = f_2k1;
        *fn1 = f_2k + f_2k1;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1  ==> 2*n >= 2, so fib_pair is safe */
    seq_t f, f1;
    fib_pair(2 * n, &f, &f1);                /* f  = F(2n),   f1 = F(2n+1) */

    seq_t lucas = (seq_t)2 * f1 - f;          /* L(2n) = 2*F(2n+1) - F(2n) */
    seq_t term  = (seq_t)n * f;              /* n*F(2n)        */
    return lucas + term;                     /* L(2n) + n*F(2n) */
}
