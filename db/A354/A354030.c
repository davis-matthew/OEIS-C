/*
 * A354030  a(n) = 1 if n is either 1 or a prime power of the form 4m+1,
 *          otherwise 0.
 * https://oeis.org/A354030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 1 or n is a prime power p^e such that p % 4 ==
 *            1 (any e) or (p % 4 == 3 && e % 2 == 0). Otherwise 0
 * Range:     u8 n <= 8848, u16 n <= 8848, u32 n <= 8848, u64 n <= 8848,
 *            u128 n <= 8848, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special offset position */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Work on a copy of n to avoid modifying the argument. */
    uint64_t m = (uint64_t)n;
    uint64_t p = 0;          /* smallest prime factor */
    uint64_t i;

    /* Trial division to find the smallest prime factor. */
    for (i = 2; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            p = i;
            break;
        }
    }
    /* If no divisor found, m itself is prime. */
    if (p == 0)
        p = m;

    /* Determine the exponent of p in the factorisation of original n. */
    uint64_t exponent = 0;
    while (m % p == 0)
    {
        m /= p;
        ++exponent;
    }

    /* If after removing all powers of p some factor remains,
       the number is not a pure prime power. */
    if (m != 1)
        return (seq_t)0;

    /* Now n = p^exponent. Check the 4m+1 condition. */
    if (p % 4 == 1)
    {
        /* All exponents work for primes ≡ 1 (mod 4). */
        return (seq_t)1;
    }
    else if (p % 4 == 3)
    {
        /* Need an even exponent for primes ≡ 3 (mod 4). */
        return (seq_t)((exponent % 2 == 0) ? 1 : 0);
    }
    else /* p == 2 */
    {
        /* Powers of 2 are never of the form 4m+1 (except 2^0=1 handled above). */
        return (seq_t)0;
    }
}
