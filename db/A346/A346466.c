/*
 * A346466  The least common multiple of all divisors d of n-1 such that d+1
 *          is a prime divisor of n; a(1) = 1.
 * https://oeis.org/A346466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM_{p-1 | n-1, p | n, p prime} (p-1)
 * Range:     u8 n <= 256, u16 n <= 8579, u32 n <= 8579, u64 n <= 8579,
 *            u128 n <= 8579, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 1 return 1; for n = 2k return 1; for n = p^e return p-1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* Provides seq_gcd if available */

static seq_t gcd_local(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)          /* a(1) = 1 */
        return (seq_t)1;

    idx_t m = n;
    idx_t primes[64];
    size_t pc = 0;

    /* Trial division to collect distinct prime divisors of n */
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == (idx_t)0)
        {
            primes[pc++] = p;
            while (m % p == (idx_t)0)
                m /= p;
        }
    }
    if (m > (idx_t)1)
        primes[pc++] = m;

    seq_t lcm = (seq_t)1;
    seq_t nm1 = (seq_t)(n - (idx_t)1);

    for (size_t i = 0; i < pc; ++i)
    {
        idx_t p = primes[i];
        seq_t p_minus = (seq_t)(p - (idx_t)1);

        if (p_minus == (seq_t)0)   /* p = 1 never occurs but keep safe */
            continue;

        if (nm1 % p_minus == (seq_t)0)
        {
            seq_t g = gcd_local(lcm, p_minus);
            lcm = (lcm / g) * p_minus;   /* lcm = lcm * p_minus / gcd(lcm,p_minus) */
        }
    }

    return lcm;
}
