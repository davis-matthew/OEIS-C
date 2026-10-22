/*
 * A363131  Number of non-co-modes in the prime factorization of n.
 * https://oeis.org/A363131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of n whose exponent is
 *            larger than the minimal exponent in the prime factorization of n
 * Range:     u8 n <= 7780, u16 n <= 7780, u32 n <= 7780, u64 n <= 7780,
 *            u128 n <= 7780, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return (seq_t)0;

    /* Factor n using the provided utility */
    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(tmp, primes, exps, &cnt);

    if (cnt == 0)      /* n is 0 or 1 */
        return (seq_t)0;

    /* Find the minimal exponent */
    uint64_t min_exp = UINT64_MAX;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] < min_exp)
            min_exp = exps[i];

    /* Count how many exponents exceed the minimum */
    uint64_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] > min_exp)
            ++res;

    return (seq_t)res;
}
