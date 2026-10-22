/*
 * A326691  a(n) = n/denominator(Sum_{prime p | n} 1/p - 1/n).
 * https://oeis.org/A326691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n/denominator(Sum_{prime p | n} 1/p - 1/n)
 * Range:     u8 n <= 255, u16 n <= 9591, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd for seq_t values */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return 1;

    uint64_t tmp = (uint64_t)n;
    seq_t N = 0;                       /* sum of n/p over distinct primes */

    /* trial‑division factorisation of tmp */
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            N += (seq_t)n / (seq_t)p;  /* add n/p once per distinct prime */
            while (tmp % p == 0)
                tmp /= p;              /* skip all copies of p */
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1)
        N += (seq_t)n / (seq_t)tmp;

    N = N - 1;  /* N now equals (sum n/p) - 1 */

    return gcd_seq(N, (seq_t)n);
}
