/*
 * A363753  a(n) = Sum_{k=0..n} (-1)^k*F(k-1)*F(k)*F(k+1)/2, where F(n) is the
 *          Fibonacci number A000045(n).
 * https://oeis.org/A363753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363753: a(n) = Sum_{k=0..n} (-1)^k*F(k-1)*F(k)*F(k+1)/2
   Implemented via the compact closed‑form:
   a(n) = ((-1)^n*(F(n+1)^3 - F(n)^3) + F(n+2) - 2)/8
   where F(m) is the Fibonacci number (F(0)=0, F(1)=1).  */

/* The Fibonacci routine fib(idx_t n) is defined in the shared utils. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the three consecutive Fibonacci numbers needed. */
    seq_t Fn   = fib(n);           /* F(n)   */
    seq_t Fn1  = fib(n + 1);       /* F(n+1) */
    seq_t Fn2  = fib(n + 2);       /* F(n+2) */

    /* (-1)^n as seq_t. */
    seq_t sign = ((n & 1) == 0) ? (seq_t)1 : (seq_t)-1;

    /* Cubes of the two Fibonacci values. */
    seq_t c1 = Fn1 * Fn1 * Fn1;    /* F(n+1)^3 */
    seq_t c2 = Fn  * Fn  * Fn;     /* F(n)^3   */

    /* Numerator of the formula. */
    seq_t numer = sign * (c1 - c2) + Fn2 - (seq_t)2;

    /* Exact division by 8. */
    return numer / (seq_t)8;
}
