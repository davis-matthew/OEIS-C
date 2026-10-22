/*
 * A349170  a(n) = Sum_{d|n} d * A003959(n/d), where A003959 is fully
 *          multiplicative with a(p) = (p+1).
 * https://oeis.org/A349170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e||n} ((p+1)^{e+1} - p^{e+1})
 * Range:     u8 n <= 23, u16 n <= 575, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset starting at 1 */
    if (n == 0)
        return 0;

    seq_t result = 1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            seq_t term1 = big_pow((seq_t)(p + 1), (seq_t)(e + 1));
            seq_t term2 = big_pow((seq_t)p, (seq_t)(e + 1));
            seq_t term  = term1 - term2;
            result *= term;
        }
    }

    if (tmp > 1)        /* Remaining prime factor with exponent 1 */
    {
        idx_t p = tmp;
        idx_t e = 1;

        seq_t term1 = big_pow((seq_t)(p + 1), (seq_t)(e + 1));
        seq_t term2 = big_pow((seq_t)p, (seq_t)(e + 1));
        seq_t term  = term1 - term2;
        result *= term;
    }

    return result;
}
