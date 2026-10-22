/*
 * A395282  a(n) = Sum_{d|n} mu(d)*Fibonacci(2*(n/d)+1).
 * https://oeis.org/A395282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(d) * Fibonacci(2*(n/d)+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, a(1) is the first term */

    /* The sum is small enough for int64_t for all n that fit into seq_t
       (for larger values the build that holds seq_t will use a larger
       integer type and the cast below will be handled by that compiler). */
    int64_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;

        /* first divisor d = i */
        idx_t d = i;
        int mu = mu_of(d);
        if (mu != 0) {
            idx_t k = 2 * (n / d) + 1;
            seq_t f = fib(k);
            if (mu == 1)
                sum += (int64_t)f;
            else            /* mu == -1 */
                sum -= (int64_t)f;
        }

        /* second divisor d = n / i, skip if equal to first */
        idx_t d2 = n / i;
        if (d2 != d) {
            int mu2 = mu_of(d2);
            if (mu2 != 0) {
                idx_t k2 = 2 * (n / d2) + 1;
                seq_t f2 = fib(k2);
                if (mu2 == 1)
                    sum += (int64_t)f2;
                else
                    sum -= (int64_t)f2;
            }
        }
    }

    return (seq_t)sum;
}
