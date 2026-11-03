/*
 * A379009  a(n) = 2*n^2 XOR 1+sigma(n^2).
 * https://oeis.org/A379009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^2) XOR (1 + sigma(n^2))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 4321, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 77 terms through n = 9306 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute sigma(n^2) using prime factorisation of n. */
static seq_t sigma_of_nsq(idx_t n)
{
    seq_t sigma = 1;
    idx_t m = n;

    /* trial division */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            /* component for prime p with exponent e in n
               gives sigma(p^(2*e)) = (p^(2*e+1)-1)/(p-1) */
            idx_t exp = 2 * e + 1;
            seq_t p_pow = 1;
            for (idx_t i = 0; i < exp; ++i)
                p_pow *= (seq_t)p;

            seq_t component = (p_pow - 1) / ((seq_t)p - 1);
            sigma *= component;
        }
    }

    /* remaining prime factor > sqrt(m) */
    if (m > 1)
    {
        /* e = 1 for this prime in n */
        seq_t p_pow = (seq_t)m * (seq_t)m * (seq_t)m; /* m^(2*1+1) = m^3 */
        seq_t component = (p_pow - 1) / ((seq_t)m - 1);
        sigma *= component;
    }

    return sigma;
}

seq_t a(idx_t n)
{
    /* compute 2*n^2 */
    idx_t nsq = n * n;
    seq_t two_nsq = ((seq_t)nsq) * 2;

    /* compute sigma(n^2) and add 1 */
    seq_t s_plus_one = sigma_of_nsq(n) + 1;

    /* XOR the two values */
    return two_nsq ^ s_plus_one;
}
