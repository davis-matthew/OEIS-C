/*
 * A362632  a(n) = Sum_{d|n, gcd(d,n/d)=1} d * psi(d), where psi is the
 *          Dedekind psi function (A001615).
 * https://oeis.org/A362632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∑_{d|n, gcd(d,n/d)=1} d * ψ(d) (ψ = Dedekind ψ)
 * Range:     u8 n <= 11, u16 n <= 149, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the multiplicative formula: for n = Π p^e, */
/* a(n) = Π (1 + (p+1) * p^(2e-1))                        */
#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) /* the sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* temporary arrays – at most 64 distinct prime factors for a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 is provided by the utilities suite */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p      = (seq_t)primes[i];
        uint64_t e   = exps[i];

        /* Compute p^(2*e-1) */
        seq_t pow_term = big_pow(p, (seq_t)(2 * e - 1));

        /* term for this prime power: 1 + (p+1) * p^(2e-1) */
        seq_t term = (seq_t)1 + (p + (seq_t)1) * pow_term;

        /* product over all prime powers */
        result *= term;
    }
    return result;
}
