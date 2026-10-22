/*
 * A362434  Numbers that can be written as A000045(i) + j^2 for i,j>=0 in 4
 *          ways.
 * https://oeis.org/A362434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (fib(6*n)/2)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* n is 1-based, offset 1 */
    idx_t six_n = 6 * n;          /* index 6*n in Fibonacci sequence */
    seq_t fib_val = fib(six_n);   /* fib(6*n) */
    seq_t half = fib_val / 2;     /* fib(6*n) is even for n>=1 */
    seq_t sq = half * half;       /* (fib(6*n)/2)^2 */
    return sq + 1;
}
