/*
 * A359038  a(n) = Sum_{d|n} tau(d^7), where tau(n) = number of divisors of n,
 *          cf. A000005.
 * https://oeis.org/A359038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} tau(d^7) (multiplicative, a(p^e) =
 *            (7e^2+9e+2)/2)
 * Range:     u8 n <= 23, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t ev = (seq_t)e;
            seq_t val = ((seq_t)7 * ev * ev + (seq_t)9 * ev + (seq_t)2) / 2;
            result *= val;
        }
    }
    if (temp > 1)        /* remaining prime factor */
    {
        seq_t ev = 1;
        seq_t val = ((seq_t)7 * ev * ev + (seq_t)9 * ev + (seq_t)2) / 2;
        result *= val;
    }
    return result;
}
