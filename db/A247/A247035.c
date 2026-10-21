/*
 * A247035  Expansion of 2*(x+1)*(x^4+6*x^3+5*x^2+6*x+1)/(x^6-18*x^3+1).
 * https://oeis.org/A247035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A247035: Expansion of 2*(x+1)*(x^4+6*x^3+5*x^2+6*x+1)/(x^6-18*x^3+1)
   Formula: a(0) = 2,
            a(n) = 2*(3*F(2n)+F(2n-1))                     if n mod 3 ≠ 1
            a(n) = (7/2)*(3*F(2n)+F(2n-1))                 if n mod 3 = 1
   where F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1).
   Uses the helper function seq_t fib(idx_t n) from ../utils/fib.h.
   All arithmetic is performed in the seq_t type. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0) {
        return (seq_t)2;
    }

    /* Compute 2*n and the required Fibonacci numbers. */
    idx_t two_n   = 2 * n;
    seq_t f2n     = fib(two_n);          /* F(2n) */
    seq_t f2n_1   = fib(two_n - 1);      /* F(2n-1) */

    /* Common factor: 3*F(2n) + F(2n-1). */
    seq_t base = ((seq_t)3 * f2n) + f2n_1;

    /* Apply the branching rule on n modulo 3. */
    if (n % 3 == 1) {
        /* 7/2 * base; base is always even, so division is exact. */
        return ((seq_t)7 * base) / 2;
    } else {
        return ((seq_t)2 * base);
    }
}
