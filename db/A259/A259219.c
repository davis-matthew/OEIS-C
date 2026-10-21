/*
 * A259219  Number of (n+1) X (5+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 19*Fibonacci(n+3) + 24
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n + 1);
    seq_t fib_val = fib(n + 3);
    seq_t result = two_pow + fib_val * 19 + 24;
    return result;
}
