/*
 * A361175  The sum of the exponential infinitary divisors of n.
 * https://oeis.org/A361175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e in n of (sum_{d infinitary
 *            divisor of e} p^d) where an infinitary divisor of e is a divisor
 *            whose prime exponents are either 0 or the corresponding exponent
 *            of e
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, computes the sum over
   exponential infinitary divisors for each prime power,
   and multiplies these sums.  All operations use the
   provided utilities for arbitrary‑precision arithmetic. */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64() */
#include "../utils/seq_pow.h"     /* seq_pow() */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t N = (uint64_t)n;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcount = 0;
    factor_u64(N, primes, exps, &pcount);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcount; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* sum over infinitary divisors of e */
        seq_t sum = seq_pow(p, (idx_t)1);   /* d = 1 gives p^1 */

        /* factor the integer e itself */
        uint64_t ep[64];
        uint64_t ef[64];
        size_t ecnt = 0;
        factor_u64(e, ep, ef, &ecnt);

        /* iterate over all non‑empty subsets of prime factors of e */
        unsigned int maskmax = (unsigned int)(1u << ecnt);
        for (unsigned int mask = 1; mask < maskmax; ++mask)
        {
            uint64_t d = 1;
            for (size_t j = 0; j < ecnt; ++j)
            {
                if (mask & (1u << j))
                {
                    uint64_t q = ep[j];
                    uint64_t f = ef[j];
                    /* compute q^f */
                    uint64_t qpow = 1;
                    for (uint64_t t = 0; t < f; ++t)
                        qpow *= q;
                    d *= qpow;   /* product of selected prime powers */
                }
            }
            sum += seq_pow(p, d);
        }

        result *= sum;
    }

    return result;
}
