/*
 * A388016  Odd squares k, multiples of 3 and non-multiples of 5, such that
 *          sigma(k)/k >= 5/3.
 * https://oeis.org/A388016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m_n)^2 where m_n is the n-th odd integer that * is a
 *            multiple of 3 but not of 5, and satisfies sigma(m_n^2)/m_n^2 >=
 *            5/3. * We generate odd multiples of 3 congruent to 3 (mod 6),
 *            skip those * divisible by 5, factor each candidate by trial
 *            division, compute * sigma(m^2) and test 3*sigma(m^...
 * Range:     u8 none, u16 n <= 4, u32 n <= 899, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute sigma(m^2) for odd m given as a 64‑bit integer.
 * The result fits in seq_t for the values used by the tests. */
static seq_t sigma_square_of(uint64_t m)
{
    seq_t sigma = 1;          /* product of sigma(p^{2e}) over all prime factors of m */
    uint64_t remaining = m;

    /* handle factor 2 only if m is even (not needed for odd m but kept generic) */
    /* trial division over odd primes */
    for (uint64_t p = 3; p <= remaining / p; p += 2)
    {
        if (remaining % p == 0)
        {
            unsigned int e = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++e;
            }

            /* term = sigma(p^{2e}) = (p^{2e+1}-1)/(p-1) */
            seq_t term = 1;
            for (unsigned int i = 0; i < 2 * e + 1; ++i)
                term *= (seq_t)p;
            term = (term - 1) / (p - 1);

            sigma *= term;
        }
    }

    if (remaining > 1)   /* remaining is a prime factor > sqrt(m) */
    {
        uint64_t p = remaining;   /* exponent e = 1 */
        seq_t term = 1;
        for (unsigned int i = 0; i < 3; ++i)   /* p^{2*1+1} = p^3 */
            term *= (seq_t)p;
        term = (term - 1) / (p - 1);
        sigma *= term;
    }

    return sigma;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* no term for offset 0 */

    size_t found = 0;                /* number of qualifying terms found so far */

    /* iterate over odd multiples of 3: 3, 9, 15, 21, ...  (step 6) */
    for (uint64_t m = 3; ; m += 6)
    {
        if (m % 5 == 0) continue;    /* skip multiples of 5 */

        /* m is odd, multiple of 3, not a multiple of 5 */
        seq_t sigma_m2 = sigma_square_of(m);
        seq_t m2 = (seq_t)m * (seq_t)m;

        /* check inequality 3 * sigma(m^2) >= 5 * m^2 */
        if (3 * sigma_m2 >= 5 * m2)
        {
            ++found;
            if (found == (size_t)n)
                return m2;
        }
    }
}
