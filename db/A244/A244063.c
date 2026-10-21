/*
 * A244063  Number of prime factors (with multiplicity) of the number of
 *          distinct prime factors of n; a(n) = Omega(omega(n)).
 * https://oeis.org/A244063
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Omega(omega(n))
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   count_distinct_primes, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of prime factors (with multiplicity) of the number of distinct prime factors of n. */

#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* behaviour below the offset is irrelevant */
    uint64_t u = (uint64_t)n;
    uint64_t omega = count_distinct_primes(u); /* omega(n) */
    seq_t omega_seq = (seq_t)omega; /* cast to seq_t for the next routine */
    return count_prime_factors(omega_seq); /* Omega(omega(n)) */
}
