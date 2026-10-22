/*
 * A360467  a(n) = Fibonacci(4*n+2) + 3*Fibonacci(2*n+1)^2.
 * https://oeis.org/A360467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4*n+2) + 3*Fibonacci(2*n+1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t f1 = fib(4*n + 2);
    seq_t f2 = fib(2*n + 1);
    seq_t tmp = f2 * f2;
    tmp = tmp * 3;
    return f1 + tmp;
}
