/*
 * A333843  Expansion of g.f.: Sum_{k>=1} k * x^(k^3) / (1 - x^(k^3)).
 * https://oeis.org/A333843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Product_{p^e||n} (p^(⌊e/3⌋+1) - 1)/(p - 1) (sum of
 *            divisors of the cube root of the largest cube divisor)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is idx_t (uint64_t). For n=1, factor_u64 will set cnt=0 and ans stays 1. */
    seq_t ans = (seq_t)1;

    /* Factorization storage */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e < 3)          /* exponent too small to contribute */
            continue;

        uint64_t t = e / 3; /* floor(e/3) */
        /* Compute (p^(t+1) - 1) / (p - 1) */
        seq_t p = (seq_t)primes[i];
        seq_t power = big_pow(p, (seq_t)(t + 1));
        seq_t term = (power - (seq_t)1) / (p - (seq_t)1);
        ans *= term;
    }

    return ans;
}
