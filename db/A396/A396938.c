/*
 * A396938  a(1) = 1, a(n) = Product_{i=1..omega(n)} prime(i)^(m_i-1), with n
 *          = Product_{i=1..omega(n)} p_i^m_i, omega = A001221, and p_i <
 *          p_(i+1).
 * https://oeis.org/A396938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=1..ω(n)} prime(i)^(m_i-1) where * n = ∏ p_i^{m_i}
 *            (p_i sorted ascending)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utilities for factorization, exponentiation and primes */
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n = 1 is a special case */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* n fits in 64 bits; factor_u64 works on uint64_t */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];   /* primes dividing n */
    uint64_t exps[64];     /* their exponents */
    size_t   cnt = 0;      /* number of distinct prime factors */

    /* Factorise nn, storing primes and exponents */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    /* For each distinct prime factor, use the i‑th prime (2,3,5,…) as the
       base.  The exponent is (m_i-1); skip if it is zero. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t m = exps[i];
        if (m <= 1) {          /* m-1 == 0 → factor 1 */
            continue;
        }
        uint64_t base = (uint64_t)get_prime((idx_t)(i + 1));   /* kth prime */
        seq_t    power = pow_uint(base, (idx_t)(m - 1));
        result *= power;
    }

    return result;
}
