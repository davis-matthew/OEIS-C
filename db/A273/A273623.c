/*
 * A273623  a(n) = Fibonacci(3*n) - (2 + (-1)^n)*Fibonacci(n).
 * https://oeis.org/A273623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(3*n) - (2 + (-1)^n)*Fibonacci(n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   fib, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 5, a(3) = 32, ... */

#include "../utils/fib.h"           /* provides seq_t fib(idx_t n)          */
#include "../utils/mul_small.h"     /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, but safe guard      */

    seq_t fn  = fib(n);                  /* Fibonacci(n)                     */
    seq_t fn3 = fib(n * 3);               /* Fibonacci(3*n)                   */

    unsigned int factor = (n % 2 == 0) ? 3u : 1u;  /* (2+(-1)^n) */
    seq_t mul = mul_small(fn, factor);   /* factor * Fibonacci(n)            */

    return fn3 - mul;                    /* result fits in seq_t             */
}
