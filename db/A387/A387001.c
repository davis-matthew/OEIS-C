/*
 * A387001  Number of vertices in the diagram called "symmetric representation
 *          of sigma(n)" where its "parts" or polygons are dissected into unit
 *          squares (see the example).
 * https://oeis.org/A387001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + 2*n + 1 * where sigma(n) is the sum of the
 *            positive divisors of n
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return sigma(n) using trial division. */
static seq_t sigma_of(idx_t n)
{
    idx_t m = n;
    seq_t sigma = (seq_t)1;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            seq_t term = (seq_t)1;   /* 1 + p + p^2 + ... + p^e */
            seq_t pow  = (seq_t)1;   /* p^e */
            while (m % p == 0)
            {
                m /= p;
                pow *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
        }
    }

    /* if m > 1, the remaining factor is prime */
    if (m > 1)
        sigma *= ((seq_t)1 + (seq_t)m);

    return sigma;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, but safe guard */

    seq_t sigma = sigma_of(n);
    seq_t term  = (seq_t)2 * (seq_t)n + (seq_t)1;
    return sigma + term;
}
