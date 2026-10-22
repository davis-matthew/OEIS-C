/*
 * A384763  Product of the Euler totients of the unitary divisors of n.
 * https://oeis.org/A384763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{d|n, gcd(d, n/d)=1} phi(d) Equivalent: a(n) =
 *            phi(n)^(2^(omega(n)-1)) (omega = number of distinct prime
 *            factors) with a(1)=1
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   calc_phi, count_distinct_primes, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_phi.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    /* Euler's totient of n. */
    seq_t phi_n = calc_phi((seq_t)n);

    /* Number of distinct prime factors of n. */
    int omega = count_distinct_primes((uint64_t)n);
    if (omega <= 0) return 1;           /* Should not happen except n==1. */

    /* exponent = 2^(omega-1). */
    idx_t exp = 1ULL << (omega - 1);

    /* Raise phi(n) to the required power. */
    return seq_pow(phi_n, exp);
}
