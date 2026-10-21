/*
 * A259451  a(n) = n^2*Fibonacci(n).
 * https://oeis.org/A259451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * Fibonacci(n)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 33, u64 n <= 75, u128 n <= 164,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to sequence type */
    return (sn * sn) * fib(n);    /* n^2 * Fib(n) */
}
