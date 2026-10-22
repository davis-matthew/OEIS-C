/*
 * A351602  a(n) = n^4 * Sum_{d^2|n} 1 / d^4.
 * https://oeis.org/A351602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^4 * Sum_{d^2 | n} 1/d^4 * Multiplicative form: a(p^e) =
 *            p^4 * (p^(4e) - p^(4⌊(e-1)/2⌋)) / (p^4 - 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Compute the contribution of a single prime power p^e */
static seq_t prime_power_term(seq_t p, unsigned e)
{
    /* p^4 */
    seq_t p4 = seq_pow(p, 4U);

    /* exponents for the numerator terms */
    idx_t exp_full = 4U * (idx_t)(e);                     /* 4e */
    idx_t exp_half = 4U * (idx_t)((e - 1U) / 2U);         /* 4⌊(e-1)/2⌋ */

    /* numerator and denominator of the fraction */
    seq_t num = seq_pow(p, exp_full) - seq_pow(p, exp_half);
    seq_t den = p4 - 1U;

    /* division is exact; avoid overflow by dividing first */
    seq_t mult = num / den;          /* (p^(4e) - p^(4⌊(e-1)/2⌋)) / (p^4-1) */
    return p4 * mult;                /* multiply by p^4 */
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1U)
        return 0U;                  /* behaviour for indices below the offset is irrelevant */

    /* factor n: primes[] and exps[] hold the prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* For n == 1 there are no prime factors */
    if (cnt == 0U)
        return 1U;

    seq_t result = 1U;
    for (size_t i = 0U; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned e = (unsigned)exps[i];
        result *= prime_power_term(p, e);
    }
    return result;
}
