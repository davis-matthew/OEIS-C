/*
 * A293066  Number of vertices at level n of the hyperbolic Pascal pyramid
 *          PP_(4,5).
 * https://oeis.org/A293066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + Fibonacci(2*n - 1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;        /* a(0) = 1 */

    /* compute 2*n in seq_t */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* compute Fibonacci at index 2*n - 1 */
    idx_t fib_index = 2 * n - 1;
    seq_t fib_val = fib(fib_index);

    return two_n + fib_val;
}
