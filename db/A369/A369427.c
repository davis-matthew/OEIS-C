/*
 * A369427  The number of unitary divisors of n that are squares of primes.
 * https://oeis.org/A369427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes whose exponent in the factorization of n
 *            equals exactly 2, i.e. the count of prime factors with exponent
 *            2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;           /* n ≥ 1 */
    uint64_t pr[64], ex[64];             /* prime factor lists (size 64 is ample) */
    size_t cnt;                          /* number of distinct prime factors */

    factor_u64(nn, pr, ex, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] == 2)
            ++res;

    return res;
}
