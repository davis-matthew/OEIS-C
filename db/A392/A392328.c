/*
 * A392328  a(n) = Fibonacci(n^2+1).
 * https://oeis.org/A392328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the index n^2 + 1 safely in idx_t  (no overflow for n <= 10^4) */
    idx_t k = n * n + 1;
    return fib(k);
}
