/*
 * A341928  a(n) = F(n+4) * F(n+2) + 7 * (-1)^n where F(n) = A000045(n) are
 *          the Fibonacci numbers.
 * https://oeis.org/A341928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n+4) * F(n+2) + 7 * (-1)^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f1 = fib(n + 4);
    seq_t f2 = fib(n + 2);
    seq_t prod = f1 * f2;
    if (n & 1)
        return prod - (seq_t)7;
    else
        return prod + (seq_t)7;
}
