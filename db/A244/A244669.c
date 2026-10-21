/*
 * A244669  Denominators of (product of divisors of n / sum of divisors of n).
 * https://oeis.org/A244669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 59, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of (product of divisors of n / sum of divisors of n) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Euclidean algorithm for seq_t values */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    /* factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* number of divisors d(n) */
    seq_t d = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        d = d * (seq_t)(exps[i] + 1);

    /* product of divisors: ∏ p_i^{e_i * d / 2} */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t exp_val = e * (uint64_t)d / 2;
        prod = prod * seq_pow((seq_t)primes[i], (idx_t)exp_val);
    }

    /* sum of divisors σ(n) = ∏ (p_i^{e_i+1}-1)/(p_i-1) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t numerator   = seq_pow((seq_t)p, (idx_t)(e + 1)) - (seq_t)1;
        seq_t denominator = (seq_t)(p - 1);
        sigma = sigma * (numerator / denominator);
    }

    /* reduce product / sigma and return denominator */
    seq_t g   = seq_gcd(prod, sigma);
    seq_t den = sigma / g;
    return den;
}
