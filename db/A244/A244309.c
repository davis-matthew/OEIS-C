/*
 * A244309  a(n) = F(n)^3 - F(n)^2, where F(n) is the n-th Fibonacci number
 *          (A000045).
 * https://oeis.org/A244309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^3 - F(n)^2, where F(n) is the n-th Fibonacci number
 *            (offset 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f  = fib(n);
    seq_t f2 = f * f;
    seq_t f3 = f2 * f;
    return f3 - f2;
}
