/*
 * A333599  a(n) = Fibonacci(n) * Fibonacci(n+1) mod Fibonacci(n+2).
 * https://oeis.org/A333599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Fibonacci(n) * Fibonacci(n+1) mod Fibonacci(n+2)
 *            Equivalent representation (from OEIS): a(2n+1) = 1, a(2n) =
 *            Fibonacci(2n+2) - 1 This follows from the identity F(k)*F(k+1)
 *            mod F(k+2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 91, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* if n is odd, answer is always 1 */
    if (n & 1U)
        return (seq_t)1;

    /* for even n, use the simplified formula */
    return fib(n + 2) - (seq_t)1;
}
