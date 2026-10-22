/*
 * A348219  a(n) = tau(n) - omega(n) + n * Sum_{p|n, p prime} 1/p.
 * https://oeis.org/A348219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n) - omega(n) + Σ_{p|n} n/p
 * Range:     u8 n <= 209, u16 n <= 9982, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   divisor_count_u64, factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = divisor_count_u64(n) - count_of_distinct_primes(n)
   + Σ_{p|n} n/p, where the sum runs over distinct prime divisors of n. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/factorize.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for 0, but harmless. */

    /* tau(n) */
    seq_t tau = (seq_t)divisor_count_u64((uint64_t)n);

    /* Factorization of n to get distinct prime divisors and exponents. */
    uint64_t primes[64];
    int exps[64];
    size_t pcount = factorize((uint64_t)n, primes, exps);

    /* Σ_{p|n} n/p */
    seq_t sumnp = 0;
    seq_t sn = (seq_t)n;
    for (size_t i = 0; i < pcount; ++i) {
        sumnp += sn / (seq_t)primes[i];
    }

    /* a(n) = tau - omega + Σ n/p */
    seq_t result = tau - (seq_t)pcount + sumnp;
    return result;
}
