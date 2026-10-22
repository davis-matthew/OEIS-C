/*
 * A346866  Sum of divisors of the n-th second hexagonal number.
 * https://oeis.org/A346866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n*(2*n+1)) = sum of divisors of the n-th second *
 *            hexagonal number
 * Range:     u8 n <= 7, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Return σ(m) where m is a uint64_t, result stored in seq_t. */
static seq_t sigma_of_uint64(uint64_t m)
{
    /* For m = 0 or 1 handle directly. */
    if (m <= 1)
        return (seq_t)m;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        uint64_t exp = exps[i];

        /* Compute 1 + p + p^2 + ... + p^exp */
        seq_t term = 1;          /* term = sum of powers for this prime */
        seq_t power = 1;         /* current p^k */
        for (uint64_t k = 0; k < exp; ++k)
        {
            power = power * p;   /* p^(k+1) */
            term = term + power;
        }
        result = result * term;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t m = nn * (2 * nn + 1);
    return sigma_of_uint64(m);
}
