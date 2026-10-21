/*
 * A276680  Number of divisors of the n-th heptagonal number.
 * https://oeis.org/A276680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A000566(n)) = number of divisors of n*(5*n-3)/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute the n-th heptagonal number H(n) = n*(5*n-3)/2 using uint64_t. */
    uint64_t H = (uint64_t)n * ((uint64_t)5 * n - 3) / 2;

    /* Return the divisor count τ(H). */
    return divisor_count_u64(H);
}
