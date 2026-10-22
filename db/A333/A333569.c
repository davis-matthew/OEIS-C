/*
 * A333569  a(n) = Sum_{d|n} (-1)^(bigomega(d) - omega(d)) * phi(n/d).
 * https://oeis.org/A333569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(bigomega(d)-omega(d)) * φ(n/d) * Using
 *            multiplicativity: * a(p) = p * a(p^e) = (p^e*(p^2+p-2) -
 *            2*(-1)^e*p) / (p*(p+1)) for e>1
 * Range:     u8 n <= 24, u16 n <= 2889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0 but harmless */
    seq_t result = 1;                /* multiplicative identity */
    idx_t temp   = n;

    /* trial division for all primes up to sqrt(temp) */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* count exponent e of prime p in factorization of n */
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* compute a(p^e) */
            seq_t term;
            if (e == 1)
            {
                term = (seq_t)p;          /* a(p) = p */
            }
            else
            {
                /* pe = p^e as seq_t */
                seq_t pe = 1;
                for (idx_t i = 0; i < e; ++i) pe *= (seq_t)p;

                /* numerator = p^e*(p^2+p-2) - 2*(-1)^e*p */
                seq_t numerator = pe * (((seq_t)p * (seq_t)p) + p - 2);
                if (e & 1)          /* e odd: (-1)^e = -1  -> subtract(-2p) = +2p */
                    numerator += 2 * (seq_t)p;
                else                /* e even: (-1)^e = 1   -> subtract(2p) */
                    numerator -= 2 * (seq_t)p;

                /* denominator = p*(p+1) */
                seq_t denominator = (seq_t)p * (p + 1);

                term = numerator / denominator;   /* integer division is exact */
            }

            result *= term;      /* multiplicative combination */
        }
    }

    /* if leftover prime factor > sqrt(original n) */
    if (temp > 1)
    {
        /* exponent is 1, so a(temp) = temp */
        result *= (seq_t)temp;
    }

    return result;
}
