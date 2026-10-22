/*
 * A304768  Augmented integer conjugate of n. a(n) = (1/n) * A007947(n)^(1 +
 *          A051903(n)) where A007947 is squarefree kernel and A051903 is
 *          maximum prime exponent.
 * https://oeis.org/A304768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p^e||n} p^(maxExp - e + 1) where maxExp is the
 *            maximum exponent in the prime factorisation of n
 * Range:     u8 n <= 51, u16 n <= 239, u32 n <= 2367, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Enough slots for the prime factors of any 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n */
    factor_u64(n, primes, exps, &cnt);

    /* Find the maximum exponent */
    uint64_t maxExp = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > maxExp) maxExp = exps[i];

    /* Build the product */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t newExp = maxExp - exps[i] + 1;      /* exponent for this prime */
        seq_t powVal = big_pow((seq_t)primes[i], (seq_t)newExp);
        result = result * powVal;                    /* exact multiplication */
    }
    return result;
}
