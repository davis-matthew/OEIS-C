/*
 * A244855  a(n) = Fibonacci(n)^4-1.
 * https://oeis.org/A244855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)^4 - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t f = fib(n);          /* Fibonacci(n) */
    seq_t f2 = f * f;          /* Fibonacci(n)^2     */
    seq_t f4 = f2 * f2;        /* Fibonacci(n)^4     */
    return f4 - (seq_t)1;      /* subtract 1          */
}
