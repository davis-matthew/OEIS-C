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
 * Validated: standard, pass, 65 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* if n is odd, answer is always 1 */
    if (n & 1U)
        return (seq_t)1;

    /* for even n, use the simplified formula */
    return fib(n + 2) - (seq_t)1;
}
