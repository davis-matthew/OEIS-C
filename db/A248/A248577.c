/*
 * A248577  Product of the number of divisors of n and the number of distinct
 *          prime divisors of n; i.e., tau(n) * omega(n).
 * https://oeis.org/A248577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) * omega(n) where tau(n) = number of divisors of n
 *            omega(n) = number of distinct prime factors of n Computed using
 *            helper functions divisor_count_u64 and count_distinct_primes
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"


seq_t a(idx_t n)
{
    /* OEIS offset is 1: a(1) is the first term.
       For n = 0 the behaviour is irrelevant, but we return 0 safely. */
    if (n == 0)
        return 0;

    seq_t tau   = divisor_count_u64((uint64_t)n);
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    return tau * omega;
}
