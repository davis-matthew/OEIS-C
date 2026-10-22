/*
 * A351519  a(n) = tau(n) + pi(n) - omega(n).
 * https://oeis.org/A351519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) + pi(n) - omega(n)
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, count_distinct_primes, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/compute_pi.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n < 1) {
        return 0;           /* undefined for n = 0, return 0 safely */
    }

    /* Classically: tau(n) = number of positive divisors of n */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* pi(n) = number of primes ≤ n */
    seq_t pi = compute_pi(n);

    /* omega(n) = number of distinct prime factors of n */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* a(n) = tau + pi - omega */
    return tau + pi - omega;
}
