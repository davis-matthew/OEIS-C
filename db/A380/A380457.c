/*
 * A380457  Sum of divisors of n plus the number of distinct prime divisors of
 *          n: a(n) = sigma(n) + omega(n).
 * https://oeis.org/A380457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) + omega(n) = Sum_{d|n} d + number of distinct
 *            prime factors of n
 * Range:     u8 n <= 48, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = sigma(n) + omega(n) */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;                 /* out of sequence range */

    if (n == (idx_t)1)
        return (seq_t)1;          /* sigma(1)=1, omega(1)=0 */

    uint64_t temp = (uint64_t)n;
    seq_t   sigma = (seq_t)1;     /* multiplicative identity */
    unsigned omega = 0;

    uint64_t p = 2;
    while (p <= temp / p)        /* trial division up to sqrt(temp) */
    {
        if (temp % p == 0)
        {
            unsigned e = 0;
            /* remove all factors of p */
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* term = (p^(e+1) - 1) / (p - 1)   (geometric series) */
            seq_t power = (seq_t)1;
            for (unsigned i = 0; i < e + 1; ++i)
                power *= (seq_t)p;
            seq_t term = (power - (seq_t)1) / ((seq_t)p - (seq_t)1);

            sigma *= term;
            ++omega;
        }
        ++p;
    }

    /* if a prime factor remains > sqrt(original n) */
    if (temp > 1)
    {
        seq_t term = (seq_t)temp + (seq_t)1;   /* sigma of a prime p is 1+p */
        sigma *= term;
        ++omega;
    }

    return sigma + (seq_t)omega;
}
