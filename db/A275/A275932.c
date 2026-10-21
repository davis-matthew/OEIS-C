/*
 * A275932  a(n) = F(2*n+6)*F(2*n+2)^3, where F = Fibonacci (A000045).
 * https://oeis.org/A275932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(2*n+6) * F(2*n+2)^3 where F(k) is the k‑th Fibonacci
 *            number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

/* Computes a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Compute the required Fibonacci indices. */
    idx_t m1 = 2*n + 6;   /* F(2*n+6) */
    idx_t m2 = 2*n + 2;   /* F(2*n+2) */

    /* Retrieve the Fibonacci numbers. */
    seq_t f1 = fib(m1);
    seq_t f2 = fib(m2);

    /* Compute f2^3 and return the product. */
    seq_t cube = f2 * f2 * f2;
    return f1 * cube;
}
