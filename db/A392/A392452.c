/*
 * A392452  The number of coreful infinitary divisors of n that are numbers
 *          whose prime factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A392452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e || n of popcount(e)
 *            (multiplicative: a(p^e) = popcount(e))
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
    /* Handle n = 0 gracefully (though offset starts at 1) */
    if (n == 0)
        return 0;

    uint64_t nn = (uint64_t)n;

    /* buffers for prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factorize nn; cnt will receive the number of distinct primes */
    factor_u64(nn, primes, exps, &cnt);

    /* accumulate product of popcounts of the exponents */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned int pc = __builtin_popcountll(exps[i]); /* popcount of exponent */
        result *= (seq_t)pc;
    }
    return result;
}
