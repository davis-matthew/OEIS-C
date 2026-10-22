/*
 * A383717  Dirichlet g.f.: Product_{p prime} (1 + 1/p^(s-1) + 1/p^(2*s-1)).
 * https://oeis.org/A383717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product of the distinct prime factors of n if all prime
 *            exponents ≤ 2, otherwise 0 (i.e., a(p^e) = p for e≤2, else 0)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return 1;

    uint64_t m = (uint64_t)n;      /* mutable copy, fits in 64‑bit */
    seq_t product = 1;             /* accumulated product of primes */

    /* Factorise m, testing all integers up to √m.
       Using p <= m/p to avoid overflow.                     */
    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            unsigned exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
                if (exp > 2)          /* exponent > 2 ⇒ result 0 */
                    return 0;
            }
            product *= (seq_t)p;       /* include prime once */
        }
    }

    /* If a prime factor > √original_n remains, it has exponent 1. */
    if (m > 1)
        product *= (seq_t)m;

    return product;
}
