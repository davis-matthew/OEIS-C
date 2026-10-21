/*
 * A253560  Multiply n by its largest prime factor: a(n) = A006530(n) * n.
 * https://oeis.org/A253560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * largest_prime_factor(n)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    uint64_t np = (uint64_t)n;
    uint64_t lpf = largest_prime_factor(np);
    seq_t result = (seq_t)np;
    result *= (seq_t)lpf;
    return result;
}
