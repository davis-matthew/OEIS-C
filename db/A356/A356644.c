/*
 * A356644  Number of vertex cuts in the n-antiprism graph.
 * https://oeis.org/A356644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n + 2*n - Lucas(2*n) - 2*n*Fib(2*n) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   fib, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast Fibonacci (fib) and fast exponentiation (pow_uint) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* offset starts at 1 */

    /* 4^n */
    seq_t pow4 = pow_uint(4, n);

    /* Fibonacci terms */
    idx_t two_n      = n * (idx_t)2;
    idx_t two_n_m1   = two_n - (idx_t)1;
    idx_t two_n_p1   = two_n + (idx_t)1;

    seq_t fib2n      = fib(two_n);
    seq_t fib2n_m1   = fib(two_n_m1);
    seq_t fib2n_p1   = fib(two_n_p1);

    /* Lucas(2*n) = Fib(2*n-1) + Fib(2*n+1) */
    seq_t lucas2n = fib2n_m1 + fib2n_p1;

    /* compute the formula step by step */
    seq_t term = pow4;                           /* 4^n */
    term += (seq_t)2 * n;                        /* + 2*n */
    term -= lucas2n;                             /* - Lucas(2*n) */
    term -= ((seq_t)2 * n) * fib2n;              /* - 2*n*Fib(2*n) */
    term -= (seq_t)1;                            /* - 1 */

    return term;
}
