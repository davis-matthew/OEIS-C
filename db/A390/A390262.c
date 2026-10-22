/*
 * A390262  Multiplicative sequence a(n) with a(p^e) = ((p + 1)^2 * p^(e-1) -
 *          2) * p^(e-1) for prime p and e > 0.
 * https://oeis.org/A390262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative sequence defined by a(p^e) = ((p + 1)^2 * p^(e-1)
 *            – 2) * p^(e-1) for prime p and e > 0
 * Range:     u8 n <= 11, u16 n <= 139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64 implementation is provided by the build */

/* Computes a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;    /* a(1) = 1 */

    /* Obtain prime factorisation of n. 64 slots are ample for uint64_t. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    seq_t result = 1;

    for (size_t i = 0; i < count; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned e = (unsigned)exps[i];   /* e > 0 */

        /* p_pow = p^(e-1) */
        seq_t p_pow = 1;
        for (unsigned k = 1; k < e; ++k)
            p_pow *= p;

        /* term = ((p + 1)^2 * p_pow^2) – 2 * p_pow */
        seq_t p_pow_sq = p_pow * p_pow;
        seq_t t1 = (p + 1) * (p + 1);
        seq_t term = t1 * p_pow_sq - 2 * p_pow;

        result *= term;   /* multiplicative property */
    }

    return result;
}
