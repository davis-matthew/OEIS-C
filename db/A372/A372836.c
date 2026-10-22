/*
 * A372836  a(n) is the numerator of Sum_{d|n, 1 < d < n} 1/d.
 * https://oeis.org/A372836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{d|n, 1<d<n} 1/d. For each n, let σ(n) be
 *            the sum of all divisors of n. Then Σ_{d|n} 1/d = σ(n)/n, so
 *            Σ_{1<d<n} 1/d = (σ(n)−n−1)/n. The fraction is reduced and we
 *            return its numerator
 * Range:     u8 n <= 167, u16 n <= 8713, u32 n <= 8713, u64 n <= 8713,
 *            u128 n <= 8713, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>

/* Compute σ(p^e) = 1 + p + p² + … + p^e. */
static seq_t sigma_prime_power(uint64_t p, int e)
{
    seq_t sum  = 1;
    seq_t term = 1;
    for (int i = 0; i < e; ++i)
    {
        term *= (seq_t)p;
        sum  += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 */

    uint64_t m = (uint64_t)n;
    seq_t sigma = 1;            /* σ(1) = 1 */

    /* Factor m by trial division. */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            sigma *= sigma_prime_power(p, exp);
        }
    }
    /* If a prime factor > sqrt(m) remains. */
    if (m > 1)
        sigma *= sigma_prime_power(m, 1);

    /* Numerator of (σ(n)−n−1)/n. */
    seq_t num = sigma - (seq_t)n - (seq_t)1;
    if (num == 0)
        return 0;                /* terms for n prime give 0 */

    seq_t g = gcd(num, (seq_t)n);
    return num / g;              /* reduce the fraction */
}
