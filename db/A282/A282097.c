/*
 * A282097  Coefficients in q-expansion of (3*E_2*E_4 - 2*E_6 - E_2^3)/1728,
 *          where E_2, E_4, E_6 are the Eisenstein series shown in A006352,
 *          A004009, A013973, respectively.
 * https://oeis.org/A282097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^2 * sigma(n) for n > 0 a(0) = 0 where sigma(n) is the
 *            sum of all positive divisors of n
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sigma_uint64(uint64_t n)
{
    seq_t sum = 1;                /* sigma(1) = 1 */
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            /* factor p^e */
            uint64_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            /* compute 1 + p + p^2 + ... + p^e */
            seq_t term = 1;          /* p^0 */
            seq_t p_pow = 1;         /* current power of p */
            for (uint64_t i = 1; i <= e; ++i)
            {
                p_pow *= (seq_t)p;   /* p^i */
                term += p_pow;       /* add to sum */
            }
            sum *= term;             /* multiplicative property */
        }
    }
    if (t > 1)   /* t is prime */
    {
        sum *= (1 + (seq_t)t);      /* sigma of prime = 1 + p */
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* compute sigma(n) */
    seq_t s = sigma_uint64((uint64_t)n);
    seq_t nn = (seq_t)n;
    return nn * nn * s;
}
