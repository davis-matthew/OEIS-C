/*
 * A250097  Denominator of the harmonic mean of the prime factors, without
 *          multiplicity, of n.
 * https://oeis.org/A250097
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the harmonic mean of the distinct prime
 *            factors of n
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  n >= 2.  For n < 2 the function returns 0. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 */

    /* ---------- factor n into its distinct prime divisors ---------- */
    size_t primes[64];                   /* n <= 2^64 so at most 64 primes */
    size_t count = 0;
    uint64_t temp = (uint64_t)n;
    uint64_t p;

    for (p = 2; p <= temp / p; ++p)
    {
        if ((temp % p) == 0)
        {
            primes[count++] = p;
            while ((temp % p) == 0)
                temp /= p;               /* remove multiplicity */
        }
    }
    if (temp > 1)                         /* remaining prime factor */
        primes[count++] = temp;

    /* ---------- compute P = product of distinct primes ---------- */
    seq_t P = 1;
    for (size_t i = 0; i < count; ++i)
        P *= (seq_t)primes[i];

    /* ---------- compute S = sum of (P / prime) ---------- */
    seq_t S = 0;
    for (size_t i = 0; i < count; ++i)
        S += P / (seq_t)primes[i];

    /* ---------- numerator = count * P ---------- */
    seq_t num = (seq_t)count * P;

    /* ---------- reduce the fraction and return the denominator ---------- */
    seq_t g = gcd(num, S);
    return S / g;
}
