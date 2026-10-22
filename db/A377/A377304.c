/*
 * A377304  a(n) is the number of distinct cuboids whose edges are divisors of
 *          n.
 * https://oeis.org/A377304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(tau(n)+2,3) = (tau(n)^3 + 3*tau(n)^2 +
 *            2*tau(n))/6
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility that provides a fast divisor-count routine */
#include "../utils/divisor_count_u64.h"

/* Compute a(n) for the sequence A377304 */
seq_t a(idx_t n)
{
    /* tau(n) – number of divisors of n */
    uint64_t tau = divisor_count_u64((uint64_t)n);

    /* Work in seq_t to allow arbitrary-precision if required */
    seq_t t = (seq_t)tau;

    /* a(n) = t * (t+1) * (t+2) / 6  (binomial(t+2,3)) */
    seq_t ans = t * (t + 1) * (t + 2) / 6;

    return ans;
}
