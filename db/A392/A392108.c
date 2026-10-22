/*
 * A392108  a(n) = smallest prime with exponent > 1 in the prime factorization
 *          of n, or 0 if no such prime exists.
 * https://oeis.org/A392108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime with exponent > 1 in the prime
 *            factorization of n, or 0 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (nn < 2)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    seq_t min_prime = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] > 1)
        {
            seq_t p = (seq_t)primes[i];
            if (min_prime == 0 || p < min_prime)
                min_prime = p;
        }
    }

    return min_prime;
}
