/*
 * A386259  Exponent of the highest power of 2 dividing the maximum exponent
 *          in the prime factorization of n; a(1) = 0.
 * https://oeis.org/A386259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 dividing the maximum exponent in n's prime
 *            factorization
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A386259) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* factor n and store primes & exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* find maximum exponent */
    seq_t maxExp = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t e = (seq_t)exps[i];
        if (e > maxExp)
            maxExp = e;
    }

    /* exponent of 2 dividing maxExp (v2(maxExp)) */
    seq_t res = 0;
    while ((maxExp & 1) == 0 && maxExp != 0)
    {
        maxExp >>= 1;
        ++res;
    }

    return res;
}
