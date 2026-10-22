/*
 * A345059  a(n) = A129283(n) / gcd(sigma(n), A129283(n)), where A129283(n) is
 *          the sum of n and its arithmetic derivative.
 * https://oeis.org/A345059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9633, u32 n <= 9633, u64 n <= 9633,
 *            u128 n <= 9633, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* POEMCT: a(n) = A129283(n) / gcd(sigma(n), A129283(n))
   A129283(n) = n + n * Σ_{p|n} v_p(n) / p
   where v_p(n) is the exponent of prime p in n. */

/* Precision helpers provided by the harness */
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 0 for n = 0 just for safety. */
    if (n == 0)
        return 0;

    /* Work in seq_t for all arithmetic to match the required type. */
    seq_t n0 = (seq_t)n;

    /* Compute the arithmetic derivative of n. */
    seq_t deriv = 0;
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[], and cnt with the prime factorisation
       of temp. */
    factor_u64(temp, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* term = e * (n / p)   – all in seq_t */
        seq_t term = (seq_t)e * (n0 / (seq_t)p);
        deriv += term;
    }

    /* A129283(n) = n + deriv */
    seq_t A129283 = n0 + deriv;

    /* σ(n) – sum of divisors of n */
    seq_t sigma_n = sigma_of(n);

    /* gcd and division */
    seq_t g = gcd(A129283, sigma_n);
    return A129283 / g;
}
