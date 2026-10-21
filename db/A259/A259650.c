/*
 * A259650  Largest prime factor of the n-th pentagonal number (A000326).
 * https://oeis.org/A259650
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of n*(3*n-1)/2
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* offset is 2, value for n<2 is not used */

    uint64_t nn  = (uint64_t)n;
    uint64_t pent = nn * (3 * nn - 1) / 2;   /* pentagonal number */
    uint64_t lpf  = largest_prime_factor(pent);

    return (seq_t)lpf;
}
