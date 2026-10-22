/*
 * A361012  Multiplicative with a(p^e) = sigma(e), where sigma = A000203.
 * https://oeis.org/A361012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of sigma(exponent) over all prime powers in n
 *            where sigma(e) = sum of divisors of e (i.e., A000203)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_exp.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                      /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

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
            result *= sigma_exp(e);
        }
    }

    if (m > 1)                  /* remaining prime factor with exponent 1 */
    {
        result *= sigma_exp(1); /* sigma(1) = 1 */
    }

    return result;
}
