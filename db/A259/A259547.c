/*
 * A259547  a(n) = n^4*Fibonacci(n).
 * https://oeis.org/A259547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*Fib(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 22, u64 n <= 59, u128 n <= 144,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) */
    seq_t f = fib(n);

    /* Compute n^4 */
    seq_t t = (seq_t)n;
    t *= t;   /* n^2 */
    t *= t;   /* n^4 */

    /* Return n^4 * Fibonacci(n) */
    return t * f;
}
