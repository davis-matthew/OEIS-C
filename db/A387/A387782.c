/*
 * A387782  Lower (1/3)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( F(2*n) / 3 ), where F is the Fibonacci sequence
 *            with F(0)=0, F(1)=1. The index n here follows the OEIS offset 1:
 *            a(1)=0, a(2)=1, a(3)=2, …
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the Fibonacci routine supplied in the common utilities. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute F(2*n).  n is an unsigned 64‑bit integer, so left shift by 1
       safely yields 2*n (the shift is defined for unsigned values). */
    idx_t k = n << 1;          /* k = 2*n */

    /* fib(k) returns the k-th Fibonacci number as seq_t. */
    seq_t f = fib(k);

    /* Integer division truncates toward zero for non‑negative values,
       giving the required floor. */
    return f / 3;
}
