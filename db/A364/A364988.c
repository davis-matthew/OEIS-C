/*
 * A364988  a(n) is the sum of coreful divisors d of n such that n/d is also a
 *          coreful divisor.
 * https://oeis.org/A364988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d | n , rad(d)=rad(n)} d. For non‑powerful n the sum
 *            is 0, and a(1)=1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the factorisation of n and the multiplicative
   formula a(p^e) = (p^e-1)/(p-1) - 1 for e>=2, returning 0 if any
   exponent equals 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n==1 is a special case (the empty product). */
    if (n == 1)
        return 1;

    /* tmp is used for factorisation; use the native 64‑bit type of idx_t. */
    uint64_t tmp = n;

    /* result starts at 1; it will be multiplied by the local
       contributions for each prime factor when the exponent is at least 2. */
    seq_t result = 1;

    /* Trial division up to sqrt(tmp).  The condition p <= tmp/p
       avoids overflow of p*p for large numbers. */
    for (uint64_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)
        {
            /* Count the exponent e of prime p. */
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            /* If the exponent is 1, n is not powerful: return 0. */
            if (e == 1)
                return 0;

            /* Compute p^e in seq_t to keep the intermediate value exact. */
            seq_t pe = 1;
            for (int i = 0; i < e; ++i)
                pe *= (seq_t)p;

            /* Local term for prime p: (p^e-1)/(p-1) - 1. */
            seq_t term = (pe - 1) / ((seq_t)p - 1) - 1;

            result *= term;
        }
    }

    /* If a prime factor remains and its exponent is 1, n is not powerful. */
    if (tmp > 1)
        return 0;

    return result;
}
