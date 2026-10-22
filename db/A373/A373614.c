/*
 * A373614  a(n) = Fibonacci(n)^2 * Catalan(n).
 * https://oeis.org/A373614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fibonacci(n)^2 * catalan(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   catalan, fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where fibonacci(0)=0, fibonacci(1)=1 */

#include "../utils/catalan.h"
#include "../utils/fibonacci.h"

seq_t a(idx_t n)
{
    seq_t fib_n = fibonacci(n);
    seq_t fib_sq = fib_n * fib_n;
    seq_t cat = catalan(n);
    return fib_sq * cat;
}
