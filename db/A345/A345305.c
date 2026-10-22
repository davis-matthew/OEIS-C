/*
 * A345305  a(n) = n * Sum_{p|n, p prime} gcd(p,n/p) / p.
 * https://oeis.org/A345305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{p|n, p prime} gcd(p, n/p) / p
 * Range:     u8 n <= 143, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1) return 0;

    /* Keep the original value in seq_t for exact arithmetic. */
    seq_t orig = (seq_t)n;
    seq_t result = 0;

    /* Factor n with trial division. */
    uint64_t m = n;           /* mutable copy for division */
    uint64_t limit = n;       /* loop upper bound: sqrt(n) */

    for (uint64_t p = 2; p * p <= limit; ++p)
    {
        if (m % p == 0)                     /* p is a prime divisor of n */
        {
            seq_t term;
            /* gcd(p, n/p) is p iff p^2 divides n, otherwise 1. */
            if (orig % ((seq_t)p * (seq_t)p) == 0)
                term = orig;                /* contributes n */
            else
                term = orig / (seq_t)p;     /* contributes n/p */

            result += term;

            /* Remove all powers of p from m. */
            while (m % p == 0)
                m /= p;
        }
    }

    /* If any prime factor > sqrt(n) remains it appears exactly once. */
    if (m > 1)
    {
        /* For this prime r, p^2 never divides n, so it contributes n/r. */
        result += orig / (seq_t)m;
    }

    return result;
}
