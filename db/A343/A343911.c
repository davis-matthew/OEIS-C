/*
 * A343911  a(n) = Omega(phi(n)), where Omega is the number of prime factors
 *          of n with multiplicity and phi is the Euler totient function.
 * https://oeis.org/A343911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Omega(phi(n))
 * Range:     u8 n <= 978, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    seq_t phi_n = calc_phi((seq_t)n);
    return count_prime_factors(phi_n);
}
