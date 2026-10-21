/*
 * A272123  a(n) = Fibonacci(3n) - Fibonacci(2n).
 * https://oeis.org/A272123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(3n) - Fib(2n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    idx_t two_n   = n << 1;      /* 2*n */
    idx_t three_n = two_n + n;   /* 3*n */
    seq_t f2 = fib(two_n);
    seq_t f3 = fib(three_n);
    return f3 - f2;
}
