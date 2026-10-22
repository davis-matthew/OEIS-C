/*
 * A301819  Number of nX4 0..1 arrays with every element equal to 1 or 2
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*Fibonacci(2*n-1) + n*(n-1)/2 - 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for n<1, but harmless */
        return 0;
    idx_t idx = 2 * n - 1;         /* argument for Fibonacci */
    seq_t f = fib(idx);            /* Fibonacci(2*n-1) */
    seq_t tri = (seq_t)n * ((seq_t)n - 1) / 2; /* n*(n-1)/2 */
    return f * (seq_t)5 + tri - (seq_t)3;
}
