/*
 * A324128  a(n) = 2*n*Fibonacci(n) + (-1)^n + 1.
 * https://oeis.org/A324128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*Fib(n) + (-1)^n + 1
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 83, u128 n <= 173,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* using the provided fib() helper function from utils/fib.h */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn = fib(n);                     /* Fib(n) */
    seq_t res = (seq_t)2 * (seq_t)n * fn;   /* 2 * n * Fib(n) */
    if ((n & 1u) == 0u)                    /* if n is even add 2 */
        res += (seq_t)2;
    return res;
}
