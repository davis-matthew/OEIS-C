/*
 * A363409  a(n) = the real part of Product_{k = 1..n} (1 + k*sqrt(-2)).
 * https://oeis.org/A363409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Re(∏_{k=1..n} (1 + k*sqrt(-2)))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1. For n≥1,
   let (a_k,b_k) be the real and imaginary coefficients of
   ∏_{i=1..k} (1 + i*sqrt(-2)) = a_k + b_k*sqrt(-2). Then:
        a_k = a_{k-1} - 2*b_{k-1}*k
        b_k = a_{k-1}*k + b_{k-1}
   The real part a(n) is returned. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a_real = (seq_t)1;   /* real part a_k */
    seq_t b_imag = (seq_t)0;   /* imaginary coefficient b_k */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_s = (seq_t)k;
        seq_t new_a = a_real - (seq_t)2 * b_imag * k_s;
        seq_t new_b = a_real * k_s + b_imag;
        a_real = new_a;
        b_imag = new_b;
    }
    return a_real;
}
