/*
 * A293810  The truncated kernel function of n: the product of distinct primes
 *          dividing n, but excluding the largest prime divisor of n.
 * https://oeis.org/A293810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(n)/A006530(n)
 * Range:     u8 n <= 1312, u16 n <= 9453, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    uint64_t k = n;                /* mutable copy of n */
    seq_t prod = 1;                /* product of distinct primes */
    uint64_t maxp = 0;             /* largest prime factor */

    /* trial division, using safe condition to avoid overflow */
    for (uint64_t p = 2; p <= k / p; ++p)
    {
        if (k % p == 0)
        {
            maxp = p;                     /* track largest prime seen so far */
            prod *= (seq_t)p;              /* multiply by distinct prime once */
            while (k % p == 0) k /= p;    /* remove all occurrences of p */
        }
    }

    /* if k > 1, then k itself is a prime factor > sqrt(original n) */
    if (k > 1)
    {
        maxp = k;
        prod *= (seq_t)k;
    }

    /* exclude the largest prime divisor */
    if (maxp > 1)
        prod /= (seq_t)maxp;

    return prod;
}
