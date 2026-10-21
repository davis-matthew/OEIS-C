/*
 * A245282  G.f.: Sum_{n>=1} Fibonacci(n+1) * x^n / (1 - x^n).
 * https://oeis.org/A245282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} Fibonacci(d+1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1, but return 0 for safety */

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += fib(d + 1);
            idx_t d2 = n / d;
            if (d2 != d) {
                sum += fib(d2 + 1);
            }
        }
    }
    return sum;
}
