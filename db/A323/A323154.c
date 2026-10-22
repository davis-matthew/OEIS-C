/*
 * A323154  a(n) = 1 if n divides A048146(n) = (sigma(n)-usigma(n)), 0
 *          otherwise.
 * https://oeis.org/A323154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n divides sigma(n) - usigma(n) where sigma = sum of
 *            divisors and usigma = sum of unitary divisors, 0 otherwise
 * Range:     u8 n <= 16, u16 n <= 885, u32 n <= 9307, u64 n <= 9307,
 *            u128 n <= 9307, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* No defined value for n == 0, return 0 safely */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Factorization of n using the provided helper */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma = (seq_t)1;
    seq_t usigma = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        /* p^e and p^(e+1) */
        seq_t p_pow_e = big_pow(p, e);
        seq_t p_pow_e1 = p_pow_e * p;

        /* Sigma multiplier: (p^(e+1) - 1) / (p - 1) */
        seq_t numerator   = p_pow_e1 - (seq_t)1;
        seq_t denominator = p - (seq_t)1;
        seq_t sigma_mul   = numerator / denominator;
        sigma *= sigma_mul;

        /* Unitary sigma multiplier: 1 + p^e */
        seq_t usigma_mul = (seq_t)1 + p_pow_e;
        usigma *= usigma_mul;
    }

    seq_t diff = sigma - usigma;
    seq_t n_seq = (seq_t)n;

    return (diff % n_seq == (seq_t)0) ? (seq_t)1 : (seq_t)0;
}
