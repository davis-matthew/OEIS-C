/*
 * A318519  a(n) = A000005(n) * A003557(n).
 * https://oeis.org/A318519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A000005(n) * A003557(n). For a prime power p^e, a(p^e) =
 *            (e+1) * p^(e-1). The sequence is multiplicative, so for n =
 *            ∏p_i^e_i a(n) = ∏ (e_i + 1) * p_i^(e_i - 1)
 * Range:     u8 n <= 127, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* behavior for n < 1 is irrelevant */
        return 0;

    idx_t tmp = n;
    seq_t result = 1;

    for (idx_t p = 2; p <= tmp / p; ++p)          /* trial division up to sqrt(tmp) */
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)                  /* count exponent of prime p */
            {
                tmp /= p;
                ++exp;
            }

            /* compute p^(exp-1) */
            seq_t p_pow = 1;
            for (idx_t i = 0; i < exp - 1; ++i)
                p_pow *= (seq_t)p;

            /* multiply by (exp+1) * p_pow */
            result *= ((seq_t)(exp + 1)) * p_pow;
        }
    }

    if (tmp > 1)                                 /* remaining prime factor > sqrt(original) */
        result *= 2;                              /* (1+1) * p^0 = 2 */

    return result;
}
