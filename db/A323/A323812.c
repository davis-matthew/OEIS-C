/*
 * A323812  a(n) = n*Fibonacci(n-2) + ((-1)^n + 1)/2.
 * https://oeis.org/A323812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Fibonacci(n-2) + ((-1)^n + 1)/2
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 86, u128 n <= 177,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Base cases: a(0) = a(1) = a(2) = 1 */
    if (n <= 2) 
        return 1;

    /* Fibonacci of n-2 */
    seq_t f = fib(n - 2);

    /* n * Fibonacci(n-2) */
    seq_t res = (seq_t)n * f;

    /* add 1 if n is even ((-1)^n + 1)/2 == 1 when n even, 0 when odd */
    if ((n & 1) == 0) 
        res += 1;

    return res;
}
