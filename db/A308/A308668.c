/*
 * A308668  a(n) = Sum_{d|n} d^(n/d+n).
 * https://oeis.org/A308668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n/d + n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Computes a(n) for the OEIS sequence A308668.
   The function works for any order of calls and uses memoization
   only if the underlying big_pow implementation requires it. */
seq_t a(idx_t n)
{
    /* no need to handle n == 0; the required domain starts at 1 */
    seq_t sum = 0;
    for (idx_t d = 1; d*d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d2 = n / d;
            /* term for divisor d: d^(n + d2) */
            seq_t exp1 = (seq_t)n + (seq_t)d2;
            sum += big_pow((seq_t)d, exp1);

            if (d2 != d)
            {
                /* term for complementary divisor d2: d2^(n + d) */
                seq_t exp2 = (seq_t)n + (seq_t)d;
                sum += big_pow((seq_t)d2, exp2);
            }
        }
    }
    return sum;
}
