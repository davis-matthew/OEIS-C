/*
 * A338506  a(n) is the number of subsets of divisors of n.
 * https://oeis.org/A338506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^tau(n) (tau(n) = number of divisors of n)
 * Range:     u8 n <= 23, u16 n <= 119, u32 n <= 839, u64 n <= 7559,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* divisor_count_u64 returns τ(n) as size_t */
    size_t d = divisor_count_u64((uint64_t)n);
    /* pow2 computes 2^exp; exponent must be an idx_t */
    return pow2((idx_t)d);
}
