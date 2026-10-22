/*
 * A349474  a(n) is the length of the continued fraction of the harmonic mean
 *          of the divisors of n.
 * https://oeis.org/A349474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of continued fraction of (n * τ(n)) / σ(n) | where
 *            τ(n) is the number of divisors of n and σ(n) is the sum of
 *            divisors of n
 * Range:     u8 n <= 35, u16 n <= 4451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Compute tau(n) and sigma(n) via prime factorisation. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= ((seq_t)exps[i] + 1);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t sum  = 1;
        seq_t pp   = 1;
        seq_t p    = (seq_t)primes[i];
        for (uint64_t j = 1; j <= exps[i]; ++j) {
            pp *= p;
            sum += pp;
        }
        sigma *= sum;
    }

    /* Harmonic mean H = n * tau / sigma. */
    seq_t num = ((seq_t)n) * tau;
    seq_t den = sigma;

    /* Continued‑fraction length via Euclidean algorithm. */
    seq_t len = 0;
    while (den != 0) {
        seq_t rem = num % den;
        num = den;
        den = rem;
        ++len;
    }
    return len;
}
