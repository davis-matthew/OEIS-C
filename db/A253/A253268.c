/*
 * A253268  Product_{k=1..n} Fibonacci(k)^k.
 * https://oeis.org/A253268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{k=1}^{n} Fibonacci(k)^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t fk   = fib(k);                 /* Fibonacci(k) */
        seq_t term = big_pow(fk, (seq_t)k);   /* (Fib(k))^k   */
        result *= term;                      /* accumulation */
    }
    return result;
}
