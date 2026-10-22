/*
 * A345355  a(n) = Sum_{p|n, p prime} p^omega(n/p).
 * https://oeis.org/A345355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{p|n, p \text{ prime}} p^{\omega(n/p)}
 * Range:     u8 n <= 101, u16 n <= 1229, u32 n <= 9737, u64 n <= 9737,
 *            u128 n <= 9737, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where \omega(m) is the number of distinct prime factors of m. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; a(1) = 0. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Factorize n to obtain its distinct prime factors and exponents. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* For each prime divisor p, compute p^{omega(n/p)}. */
    /* omega(n/p) = cnt if exponent of p > 1, else cnt-1. */
    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t omega = (exps[i] > 1) ? (seq_t)cnt : (seq_t)(cnt - 1);
        seq_t term = big_pow((seq_t)primes[i], omega);
        result += term;
    }
    return result;
}
