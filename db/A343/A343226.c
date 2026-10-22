/*
 * A343226  a(n) = gcd(sigma(n), n+A003415(n)), where A003415 is the
 *          arithmetic derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A343226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), n + A003415(n))
 * Range:     u8 n <= 63, u16 n <= 8099, u32 n <= 8099, u64 n <= 8099,
 *            u128 n <= 8099, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003415(n) = n * Σ (e_i / p_i) for n = Π p_i^e_i */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* Compute the arithmetic derivative A003415(n) */
static seq_t arithmetic_derivative(idx_t n)
{
    /* Use 64‑bit temporary for factorization */
    uint64_t nn = (uint64_t)n;
    /* Arrays large enough for all prime factors of numbers up to 10⁴ */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    factor_u64(nn, primes, exps, &count);

    seq_t deriv = 0;
    seq_t sn = (seq_t)n;
    for (size_t i = 0; i < count; ++i) {
        seq_t term = ((seq_t)exps[i]) * (sn / (seq_t)primes[i]);
        deriv += term;
    }
    return deriv;
}

/* Sequence function for A343226 */
seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t deriv = arithmetic_derivative(n);
    seq_t sum   = (seq_t)n + deriv;
    return gcd(sigma, sum);
}
