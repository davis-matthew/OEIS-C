/*
 * A379351  a(n) is the greatest prime factor of n^2 + 2.
 * https://oeis.org/A379351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime factor of n^2 + 2
 * Range:     u8 n <= 20, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n * n + 2;
    return (seq_t)largest_prime_factor(m);
}
