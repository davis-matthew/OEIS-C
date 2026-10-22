/*
 * A379590  a(n) is the number of prime divisors d of n such that 2^d - 1 is
 *          prime.
 * https://oeis.org/A379590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{d prime | d divides n and 2^d-1 is prime }|
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. count of distinct prime divisors of n that are Mersenne exponents */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* list of known Mersenne exponent primes (exponents d such that 2^d-1 is prime) */
    static const idx_t mersenne_exponents[] =
        {2ULL,3ULL,5ULL,7ULL,13ULL,17ULL,19ULL,31ULL,61ULL,89ULL,
         107ULL,127ULL,521ULL,607ULL,1279ULL,2203ULL,2281ULL,
         3217ULL,4253ULL,4423ULL,9689ULL,9941ULL};

    const size_t ME_COUNT = sizeof(mersenne_exponents)/sizeof(mersenne_exponents[0]);
    int cnt = 0;

    for (size_t i = 0; i < ME_COUNT; ++i)
    {
        if (n % mersenne_exponents[i] == 0)
            ++cnt;
    }

    return (seq_t)cnt;
}
