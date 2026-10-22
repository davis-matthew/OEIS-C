/*
 * A381794  Number of connected dominating sets in the n-trapezohedral graph.
 * https://oeis.org/A381794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 4*n + 1 + 2*(2^n - 1)*Lucas(n) - Lucas(2*n) where
 *            Lucas(0)=2, Lucas(1)=1 and Lucas(k)=Lucas(k-1)+Lucas(k-2). The
 *            sequence is implemented using the fast‑doubling Fibonacci
 *            algorithm: Lucas(k)=2*Fib(k+1)-Fib(k) for all k≥0. The
 *            implementation uses the provided utilities pow2 (2^x) and...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   fib_pair, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* seq_t and idx_t are provided by the build; no need to define them */
#include "../utils/pow2.h"     /* pow2(idx_t n) -> 2^n */
#include "../utils/fib_pair.h" /* fib_pair(idx_t n, seq_t *f_n, seq_t *f_n1) */

static seq_t lucas_value(idx_t k)
{
    seq_t f_k, f_k1;            /* f_k  = Fib(k)   */
    fib_pair(k, &f_k, &f_k1);   /* f_k1 = Fib(k+1) */

    seq_t term = f_k1 * (seq_t)2; /* 2*Fib(k+1) */
    term = term - f_k;           /* 2*Fib(k+1) - Fib(k) = Lucas(k) */
    return term;
}

seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    seq_t pow2_n   = pow2(n);      /* 2^n   */
    seq_t pow2_2n  = pow2(2 * n);  /* 2^(2n)=4^n */

    /* Compute Lucas(n) and Lucas(2n) */
    seq_t lucas_n  = lucas_value(n);
    seq_t lucas_2n = lucas_value(2 * n);

    /* Term: 4^n + 4*n + 1 */
    seq_t result = pow2_2n;
    result = result + (seq_t)4 * n;
    result = result + (seq_t)1;

    /* Middle part: 2 * (2^n - 1) * Lucas(n) */
    seq_t mid = pow2_n;
    mid = mid - (seq_t)1;
    mid = mid * (seq_t)2;
    mid = mid * lucas_n;

    result = result + mid;
    result = result - lucas_2n;

    return result;
}
