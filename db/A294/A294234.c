/*
 * A294234  Number of partitions of n into two parts such that the smaller
 *          part is nonsquarefree.
 * https://oeis.org/A294234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (1 - mu(i)^2)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the Möbius value μ(n) for n >= 1.
 * Returns  1 for an even number of distinct prime factors,
 *        -1 for an odd number of distinct prime factors,
 *         0 if n has a squared prime factor.          */
static inline int mobius(idx_t n)
{
    if (n == 1) return 1;
    int sign = 1;
    idx_t x = n;

    for (idx_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            int exp = 0;
            while (x % p == 0)
            {
                x /= p;
                ++exp;
                if (exp > 1)        /* p² divides n -> not squarefree */
                    return 0;
            }
            sign = -sign;              /* distinct prime factor found */
        }
    }
    if (x > 1)            /* remaining prime factor */
        sign = -sign;

    return sign;
}

seq_t a(idx_t n)
{
    idx_t m = n >> 1;            /* floor(n/2) */

    /* Count square‑free numbers up to m.
     * squarefree = \sum_{i=1}^{⌊√m⌋} μ(i) · ⌊m / i²⌋          */
    long long squarefree = 0;
    for (idx_t i = 1; i * i <= m; ++i)
    {
        int mu = mobius(i);
        if (mu == 0) continue;
        long long term = (long long)(m / (i * i));
        squarefree += mu * term;
    }

    /* a(n) = m – (# square‑free numbers up to m)          */
    return (seq_t)(m - squarefree);
}
