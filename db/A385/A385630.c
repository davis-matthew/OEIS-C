/*
 * A385630  a(n) = n! * Prod_{k=1..n} Fibonacci(k).
 * https://oeis.org/A385630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Prod_{k=1..n} Fibonacci(k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        result *= i;           /* factorial part */
        result *= fib(i);      /* Fibonacci part */
    }
    return result;
}
