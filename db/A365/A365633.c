/*
 * A365633  The sum of divisors of n that are terms of A072873.
 * https://oeis.org/A365633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative with a(p^e) = (p^(floor(e/p)+1) - 1)/(p -
 *            1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1U)
        return 1;

    /* factorization buffers: n has at most 64 prime factors for uint64_t */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p_u = primes[i];
        uint64_t e   = exps[i];

        /* floor(e / p) + 1 */
        uint64_t t = (e / p_u) + 1U;

        seq_t p   = (seq_t)p_u;
        seq_t pow = big_pow(p, (seq_t)t);

        /* (p^t - 1) / (p - 1) */
        seq_t num   = pow - (seq_t)1;
        seq_t denom = (seq_t)p_u - (seq_t)1;
        seq_t val   = num / denom;

        result *= val;
    }

    return result;
}
