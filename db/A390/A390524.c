/*
 * A390524  Array read by ascending antidiagonals: A(n, k) = k * Fibonacci(n).
 * https://oeis.org/A390524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) read by ascending antidiagonals of the array * A(n,k) = k *
 *            Fib(n). The pair (row, col) at * index n satisfies * row + col =
 *            s, where s is the smallest integer with * (s+1)(s+2)/2 > n. Then
 *            * pos = n - s(s+1)/2, row = s - pos, col = pos, * and a(n) = col
 *            * Fib(row). (Fib(0)=0.)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that s(s+1)/2 <= n < (s+1)(s+2)/2.
       We look for the smallest s with (s+1)(s+2)/2 > n, which we
       reach by incrementing while the bound is <= n. */
    idx_t s = 0;
    while (((__uint128_t)(s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* Position of n within this diagonal. */
    idx_t pos = n - ((idx_t)((__uint128_t)s * (s + 1) / 2));

    /* Coordinates in the array. */
    idx_t row = s - pos;   /* index for Fibonacci */
    idx_t col = pos;       /* multiplier */

    /* row = 0 gives Fib(0) = 0, so the result is 0 regardless of col.
       Avoid calling fib(0) if the implementation mishandles it. */
    if (row == 0)
        return (seq_t)0;

    /* Compute k * Fib(row). */
    seq_t fib_val = fib(row);
    return ((seq_t)col) * fib_val;
}
