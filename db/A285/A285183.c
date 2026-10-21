/*
 * A285183  Nearest integer to n*omega(n)/phi(n).
 * https://oeis.org/A285183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*omega(n)/phi(n)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute the number of distinct prime factors ω(n) */
    int omega = count_distinct_primes((uint64_t)n);

    /* Compute Euler's totient φ(n) */
    seq_t phi_val = phi(n);

    /* n*omega is small for the sequence range, but compute in seq_t */
    seq_t numerator = (seq_t)n * (seq_t)omega;

    /* Integer division: q = floor(numerator/phi_val) */
    seq_t q = numerator / phi_val;
    seq_t r = numerator % phi_val;

    /* Round to nearest integer: if 2*r ≥ phi_val, round up */
    if ((r << 1) >= phi_val)
        ++q;

    return q;
}
