/*
 * A392449  The number of exponential unitary (or e-unitary) divisors of n
 *          that are squares.
 * https://oeis.org/A392449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} ( e odd ? 0 : 2^{ω(e)-1} ) where ω(e) is the
 *            number of distinct prime factors of e. So if any exponent e is
 *            odd, a(n) = 0; otherwise each odd prime factor of e contributes
 *            a factor 2, and the prime 2 contributes none. This implements
 *            the definition of A392449
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e & 1ULL)           /* odd exponent → no square exponent */
            return (seq_t)0;

        /* Count distinct prime factors of e (ω(e)) */
        uint64_t sp[64];
        uint64_t se[64];
        size_t sco;
        factor_u64(e, sp, se, &sco);
        size_t omega = sco;

        /* term = 2^{omega-1} (omega>=1 because e>=2) */
        seq_t term = (seq_t)1;
        for (size_t j = 1; j < omega; ++j)
            term *= (seq_t)2;

        result *= term;
    }
    return result;
}
