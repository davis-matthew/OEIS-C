/*
 * A399834  The sum of divisors d of n such that gcud(d, n/d) is squarefree,
 *          where gcud is the greatest common unitary divisor.
 * https://oeis.org/A399834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sum of divisors d of n such that gcud(d, n/d) is
 *            squarefree (multiplicative: a(p^e) = (p^(e+1)-1)/(p-1) if e is 2
 *            or odd, otherwise (p^(e+1)-1)/(p-1) - p^(e/2) )
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t seq_pow_uint64(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp--)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* not used, offset 1 */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    while (m > 1)
    {
        /* factor m, find smallest prime factor p */
        uint64_t p;
        uint64_t rem = m;

        /* trial division */
        for (p = 2; p <= rem / p; ++p)
            if (rem % p == 0)
                break;
        if (p > rem / p)          /* rem is prime */
            p = rem;

        /* count exponent of p */
        unsigned e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        /* compute sigma(p^e) = 1 + p + p^2 + ... + p^e */
        seq_t sigma = 1;
        seq_t pow = 1;
        for (unsigned i = 1; i <= e; ++i)
        {
            pow *= (seq_t)p;
            sigma += pow;
        }

        seq_t factor;
        if (e == 2 || (e & 1U))   /* e is 2 or odd */
            factor = sigma;
        else                      /* even exponent > 2 */
        {
            seq_t pow_mid = seq_pow_uint64((seq_t)p, e / 2);
            factor = sigma - pow_mid;
        }

        result *= factor;
    }

    return result;
}
