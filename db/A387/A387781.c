/*
 * A387781  Upper (3/2)-midsequence of (F(2n)) and (F(2n+1)), where F=A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A387781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling((3/2) * F(2*n+2)), where F is Fibonacci (A000045)
 *            Offset: 0 (a(0) is the first term)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Compute Fibonacci number F(2*n+2). */
    idx_t idx = (n << 1) + 2;          /* 2*n + 2 */
    seq_t f   = fib(idx);              /* F(2*n+2) */

    /* a(n) = ceil( (3/2) * f ) = (3*f + 1) / 2 */
    seq_t three_f = f * 3;
    seq_t result  = (three_f + 1) / 2;

    return result;
}
