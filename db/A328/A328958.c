/*
 * A328958  a(n) = d(n) - (omega(n) * bigomega(n)), where d (number of
 *          divisors) = A000005, omega = A001221, bigomega = A001222.
 * https://oeis.org/A328958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = d(n) - omega(n) * bigomega(n) where d(n) = number of
 *            divisors (A000005), omega(n) = number of distinct prime factors
 *            (A001221), bigomega(n) = total number of prime factors counted
 *            with multiplicity (A001222)
 * Range:     i8 n <= 9203, i16 n <= 9203, i32 n <= 9203, i64 n <= 9203,
 *            i128 n <= 9203, big any n
 * Helpers:   bigomega, count_distinct_primes, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;            /* safe cast for small indices */
    seq_t divisors   = divisor_count_u64(nn);
    seq_t distinct   = (seq_t)count_distinct_primes(nn);
    seq_t total_factors = bigomega(nn);
    return divisors - distinct * total_factors;
}
