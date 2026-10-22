/*
 * A374216  The 2-adic valuation of sopfr(n), where sopfr is the sum of prime
 *          factors with repetition, A001414.
 * https://oeis.org/A374216
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814(A001414(n)) (2‑adic valuation of the sum of prime
 *            factors of n)
 * Range:     u8 n <= 6801, u16 n <= 6801, u32 n <= 6801, u64 n <= 6801,
 *            u128 n <= 6801, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;                    /* values below the offset are irrelevant */

    /* Factor n into prime powers; factor_u64 provides the primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the sum of prime factors with multiplicity: Σ p_i * e_i. */
    uint64_t sum = 0ULL;
    for (size_t i = 0; i < cnt; ++i)
        sum += primes[i] * exps[i];

    /* Compute the exponent of 2 dividing that sum. */
    unsigned long long v = 0ULL;
    while ((sum & 1ULL) == 0ULL)
    {
        sum >>= 1;
        ++v;
    }

    return (seq_t)v;
}
