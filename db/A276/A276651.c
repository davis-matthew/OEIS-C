/*
 * A276651  a(n) = numerator of Sum_{p|n} 0.d where p runs through the prime
 *          divisors of n.
 * https://oeis.org/A276651
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = numerator of Sum_{p|n} 0.d where p runs over the distinct
 *            prime divisors of n and d is the decimal representation of p
 * Range:     u8 n <= 201, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_seq.h"

/* 10^e as seq_t (e is 0 … 5 for n ≤ 10 000) */
static seq_t pow10_uint(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* number of decimal digits of a positive integer */
static size_t digit_count_uint(uint64_t x)
{
    size_t c = 0;
    do { ++c; x /= 10; } while (x);
    return c;
}

/* Euclidean gcd for seq_t */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* undefined below offset, but safe */

    /* factor n into distinct prime divisors */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[64];
    size_t pcnt = 0;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[pcnt++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[pcnt++] = temp;

    /* maximum number of digits among the primes */
    size_t maxLen = 0;
    for (size_t i = 0; i < pcnt; ++i)
    {
        size_t len = digit_count_uint(primes[i]);
        if (len > maxLen)
            maxLen = len;
    }

    /* numerator N = Σ p * 10^(maxLen - len(p)) */
    seq_t N = 0;
    for (size_t i = 0; i < pcnt; ++i)
    {
        size_t len = digit_count_uint(primes[i]);
        seq_t factor = pow10_uint((idx_t)(maxLen - len));
        N += ((seq_t)primes[i]) * factor;
    }

    seq_t D = pow10_uint((idx_t)maxLen);
    seq_t g = gcd_seq(N, D);
    return N / g;
}
