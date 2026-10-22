/*
 * A321069  Greatest prime factor of n^3+2.
 * https://oeis.org/A321069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = greatest prime factor of n^3+2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1677, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Compute n³+2 in seq_t */
    seq_t n_t   = (seq_t)n;
    seq_t value = n_t * n_t * n_t + (seq_t)2;

    /* For n up to about 10 000 the value fits in 64 bits,
       so we can safely cast to uint64_t and use the supplied helper. */
    uint64_t v     = (uint64_t)value;
    uint64_t lpf   = largest_prime_factor(v);

    return (seq_t)lpf;
}
