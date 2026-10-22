/*
 * A372252  a(n) = A000045(n+1)*A000292(n) (product of a Fibonacci number and
 *          a tetrahedral number).
 * https://oeis.org/A372252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(n+1) * n*(n+1)*(n+2)/6
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where fib(k) is the k-th Fibonacci number with fib(0)=0, fib(1)=1. */
/*  The tetrahedral number A000292(n) equals n*(n+1)*(n+2)/6. */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute tetrahedral number: t = n*(n+1)*(n+2)/6 */
    seq_t t = (seq_t)n;
    t *= (n + 1);
    t *= (n + 2);
    t /= 6;

    /* Compute Fibonacci number: f = fib(n+1) */
    seq_t f = fib(n + 1);

    /* Return product */
    return f * t;
}
