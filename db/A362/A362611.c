/*
 * A362611  Number of modes in the prime factorization of n.
 * https://oeis.org/A362611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of modes in the multiset of prime factors of n
 *            (i.e., the number of distinct primes that occur the maximum
 *            number of times in the prime factorization of n). Example: 450 =
 *            2·3³·5² → modes {3,5} → a(450)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    idx_t temp = n;
    idx_t max_exp = 0;
    idx_t mode_cnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if (exp > max_exp)
            {
                max_exp = exp;
                mode_cnt = 1;
            }
            else if (exp == max_exp)
            {
                ++mode_cnt;
            }
        }
    }

    /* if temp > 1 it is a prime factor with exponent 1 */
    if (temp > 1)
    {
        idx_t exp = 1;
        if (exp > max_exp)
        {
            max_exp = exp;
            mode_cnt = 1;
        }
        else if (exp == max_exp)
        {
            ++mode_cnt;
        }
    }

    return (seq_t)mode_cnt;
}
