/*
 * A390117  Least integer m > 1 such that m*n + 1 is a prime.
 * https://oeis.org/A390117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Least integer m > 1 such that m*n + 1 is prime
 * Range:     u8 n <= 30, u16 n <= 6759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    for (seq_t m = 2; ; ++m)
    {
        seq_t val = m * nn + 1;
        if (is_prime_small(val))
            return m;
    }
}
