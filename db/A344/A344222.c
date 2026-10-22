/*
 * A344222  a(n) = Sum_{k=1..n} tau(gcd(k,n)^4), where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A344222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = multiplicative function with a(p^e) = (p^e*(p+3)-4)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 1998, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t a_prime_power(uint64_t p, uint64_t e)
{
    seq_t pw = 1;
    for (uint64_t i = 0; i < e; ++i)
        pw = pw * (seq_t)p;
    seq_t numerator = pw * ((seq_t)p + 3) - 4;
    seq_t denom = (seq_t)(p - 1);
    return numerator / denom;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;   /* a(1) = 1 (a(0) undefined) */

    idx_t temp = n;
    seq_t result = 1;

    for (idx_t p = 2; (uint64_t)p * (uint64_t)p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            result = result * a_prime_power(p, e);
        }
    }
    if (temp > 1)  /* remaining prime factor */
        result = result * a_prime_power((uint64_t)temp, 1);

    return result;
}
