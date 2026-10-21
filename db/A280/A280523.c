/*
 * A280523  a(n) = Fibonacci(2n + 1) - n.
 * https://oeis.org/A280523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(2*n + 1) - n
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the required Fibonacci index: 2*n + 1 */
    idx_t m = 2ULL * n + 1;
    seq_t f = fib(m);          /* fib(n) returns F(n) with F(0)=0, F(1)=1 */
    return f - (seq_t)n;       /* Subtract n to obtain a(n) */
}
