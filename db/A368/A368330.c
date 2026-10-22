/*
 * A368330  The number of terms of A054743 that are unitary divisors of n.
 * https://oeis.org/A368330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e | n of 1 if e <= p, else 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence A368330: the number of terms of A054743 that are unitary divisors of n */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1, a(0) is irrelevant */
        return (seq_t)1;

    idx_t temp = n;       /* working copy of n */
    seq_t res = (seq_t)1; /* multiplicative identity */

    /* factor by trial division, terminate when p*p > temp */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            /* count multiplicity of prime p */
            while (temp % p == 0)
            {
                ++e;
                temp /= p;
            }

            /* apply the multiplicative rule */
            if (e > p)
                res *= (seq_t)2;   /* multiply by 2 when exponent exceeds the prime */
        }
    }

    /* remaining prime factor if any (temp > 1) has exponent 1 <= temp, contributes factor 1 */
    /* (thus nothing to do) */

    return res;
}
