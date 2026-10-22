/*
 * A366309  The number of infinitary divisors of n that are terms of A366243.
 * https://oeis.org/A366309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = \prod_{p^e\mid\mid n} (1 + [(e & 2) != 0])
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) counts infinitary divisors of n that are products of distinct prime squares (and 1) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1; n = 0 is not expected but handled safely */

    /* Allocate space for prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 populates primes, exps, and cnt */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] & 2)          /* e has bit 1 set → exponent 2 is allowed */
            result <<= 1;         /* multiply by 2 */
    }

    return result;
}
