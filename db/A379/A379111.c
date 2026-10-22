/*
 * A379111  a(n) = 1 if bigomega(sigma(n)) is equal to omega(n), otherwise 0.
 * https://oeis.org/A379111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (bigomega(sigma(n)) == omega(n) ? 1 : 0)
 * Range:     u8 n <= 7345, u16 n <= 8762, u32 n <= 8762, u64 n <= 8762,
 *            u128 n <= 8762, big any n
 * Helpers:   bigomega, count_distinct_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(n) = number of distinct prime factors of n */

#include "../utils/count_distinct_primes.h"
#include "../utils/sigma_of.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* omega(n): number of distinct prime factors (int) */
    int omega_n = count_distinct_primes((uint64_t)n);

    /* sigma(n): sum of divisors (seq_t) */
    seq_t sigma_n = sigma_of(n);

    /* bigomega(sigma(n)) (seq_t) */
    seq_t omega_sigma = bigomega((idx_t)sigma_n);

    /* Return 1 if the two values are equal, else 0 */
    return (omega_sigma == (seq_t)omega_n) ? (seq_t)1 : (seq_t)0;
}
