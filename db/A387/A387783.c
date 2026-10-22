/*
 * A387783  Upper (1/3)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(F(2*n+2)/3) = (F(2*n+2)+2)/3; where F is Fibonacci
 *            (F(0)=0, F(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute F(2n+2) */
    idx_t idx = 2 * n + 2;
    seq_t f = fib(idx);

    /* Ceil division by 3 for positive integers */
    return (f + (seq_t)2) / (seq_t)3;
}
