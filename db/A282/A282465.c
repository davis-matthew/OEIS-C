/*
 * A282465  a(n) = 11*Fibonacci(n+3) + Fibonacci(n-8) with n>=0.
 * https://oeis.org/A282465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45*Fibonacci(n) + Fibonacci(n+1) a(0) = 1, a(1) = 46,
 *            a(2) = 47,
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   _fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    _fib_pair(n, &fn, &fn1);
    return 45 * fn + fn1;
}
