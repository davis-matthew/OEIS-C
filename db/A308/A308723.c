/*
 * A308723  Total number of parts in all m-cyclic compositions of n (where
 *          each part of size m can be colored with one of m colors).
 * https://oeis.org/A308723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} φ(n/d) * Fibonacci(2*d)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   fib, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term, defined for n ≥ 1. */
    seq_t result = 0;

    /* iterate over divisor pairs (d, n/d) without overflow */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            /* term for divisor d: φ(n/d) * Fib(2*d) */
            seq_t phi_nd = phi((idx_t)(n / d));
            seq_t fib_2d = fib((idx_t)(2 * d));
            result += phi_nd * fib_2d;

            /* complementary divisor n/d (if distinct) */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                seq_t phi_d  = phi((idx_t)d);
                seq_t fib_2d2 = fib((idx_t)(2 * d2));
                result += phi_d * fib_2d2;
            }
        }
    }

    return result;
}
