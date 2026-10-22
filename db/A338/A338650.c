/*
 * A338650  Number of divisors of n which are greater than 6.
 * https://oeis.org/A338650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = divisor_count(n) - #{d∈{1,2,3,4,5,6} : d divides n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute number of divisors of n (tau) */
    uint64_t tau = divisor_count_u64((uint64_t)n);
    seq_t res = (seq_t)tau;

    /* Subtract divisors 1..6 that actually divide n */
    res -= 1;                /* divisor 1 always divides n */
    if (n % 2 == 0) res -= 1;
    if (n % 3 == 0) res -= 1;
    if (n % 4 == 0) res -= 1;
    if (n % 5 == 0) res -= 1;
    if (n % 6 == 0) res -= 1;

    return res;
}
