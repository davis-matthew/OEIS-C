/*
 * A351570  Arithmetic derivative of the sum of the divisors of the largest
 *          unitary divisor of n that is a square.
 * https://oeis.org/A351570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative( sigma( d ) ) where d is the
 *            largest unitary divisor of n that is a square (i.e. the product
 *            of all prime powers of n with even exponent)
 * Range:     u8 n <= 323, u16 n <= 44099, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9932 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Arithmetic derivative for 64‑bit integers */
static seq_t arithmetic_derivative_uint64(uint64_t m)
{
    if (m <= 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* contribution: e * (m / p) */
        der += (seq_t)exps[i] * (seq_t)(m / primes[i]);
    }
    return der;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* construct the largest unitary square divisor d */
    uint64_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if ((exps[i] & 1ULL) == 0) {          /* exponent even → include */
            uint64_t pw = 1;
            for (uint64_t j = 0; j < exps[i]; ++j)
                pw *= primes[i];
            d *= pw;
        }
    }

    /* sum of divisors of d */
    seq_t s = sigma_of((idx_t)d);

    /* arithmetic derivative of s (s fits in 64 bits for n ≤ 10000) */
    return arithmetic_derivative_uint64((uint64_t)s);
}
