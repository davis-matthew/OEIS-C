/*
 * A329484  Dirichlet convolution of the Liouville function with itself.
 * https://oeis.org/A329484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = tau(n) * (-1)^Omega(n), i.e., a(n) = sum_{d|n}
 *            lambda(d)*lambda(n/d) where lambda is the Liouville function
 * Range:     i8 n <= 2636, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   count_prime_factors, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    /* Number of divisors of n */
    seq_t tau = (seq_t)divisor_count_u64(n);

    /* Total number of prime factors of n (counted with multiplicity) */
    seq_t omega = count_prime_factors((seq_t)n);

    /* Sign = (-1)^omega */
    seq_t sign = (omega & 1) ? -1 : 1;

    return sign * tau;
}
