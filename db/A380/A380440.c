/*
 * A380440  a(n) = 1 if n has no squarefree divisor d such that d^2 > n,
 *          otherwise 0.
 * https://oeis.org/A380440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if rad(n)^2 <= n (i.e. max squarefree divisor ≤ √n);
 *            otherwise 0
 * Range:     u8 n <= 9734, u16 n <= 9734, u32 n <= 9734, u64 n <= 9734,
 *            u128 n <= 9734, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)          /* a(1) = 1; for completeness handle n==0 if called */
        return (seq_t)1;

    uint64_t rad = 1;        /* product of distinct prime factors of n */
    uint64_t m   = (uint64_t)n;

    /* factor out primes up to √m */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            rad *= p;
            while (m % p == 0)   /* strip all copies of p */
                m /= p;
        }
    }

    /* if a prime > √original n remains, it is a distinct factor too */
    if (m > 1)
        rad *= m;

    /* check condition rad^2 ≤ n  →  rad ≤ n / rad */
    return (seq_t)(rad <= n / rad ? 1 : 0);
}
