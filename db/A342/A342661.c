/*
 * A342661  a(n) = n * sigma(A064989(n)), where A064989 is multiplicative with
 *          a(2^e) = 1 and a(p^e) = prevprime(p)^e for odd primes p, and sigma
 *          gives the sum of the divisors of its argument.
 * https://oeis.org/A342661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * σ(A064989(n)) * A064989(n) is multiplicative with
 *            A064989(p^e)=1 if p=2, otherwise A064989(p^e)=prevprime(p)^e. *
 *            Hence the multiplicative formula * a(p^e) = (p^e) *
 *            (q^(e+1)-1)/(q-1), * where q = 1 for p=2, and q = prevprime(p)
 *            for odd primes. * This function implements that formula
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 7296, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/factor_u64.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 (OEIS offset is 1). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t    p_pow = 1;

        /* p^e */
        for (uint64_t j = 0; j < e; ++j)
            p_pow *= (seq_t)p;

        seq_t term = 0;

        if (p == 2)
        {
            /* a(2^e) = 2^e */
            term = p_pow;
        }
        else
        {
            idx_t q = prev_prime(p);           /* previous prime of p */
            seq_t q_se = (seq_t)q;

            /* q^(e+1) */
            seq_t q_pow = 1;
            for (uint64_t j = 0; j < e + 1; ++j)
                q_pow *= q_se;

            /* term = p^e * (q^(e+1)-1)/(q-1) */
            seq_t numerator = q_pow - 1;
            seq_t denom     = q_se - 1;
            term = (p_pow * numerator) / denom;
        }

        result *= term;
    }

    return result;
}
