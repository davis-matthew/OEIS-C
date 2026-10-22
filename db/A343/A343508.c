/*
 * A343508  a(n) = Sum_{k=1..n} gcd(k, n)^6.
 * https://oeis.org/A343508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d^6 (OEIS A343508)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1621,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute x^6 as seq_t */
static seq_t pow6(seq_t x)
{
    seq_t r = x;
    for (int i = 0; i < 5; ++i) r *= x;
    return r;
}

/* Euler's totient for an unsigned 64‑bit integer */
static seq_t phi_uint64(uint64_t m)
{
    seq_t res = m;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            res = res / p * (p - 1);
        }
    }
    if (temp > 1) res = res / temp * (temp - 1);
    return res;
}

/* Main routine */
seq_t a(idx_t n)
{
    seq_t result = 0;
    /* enumerate divisors of n */
    for (idx_t i = 1; ((unsigned __int128)i * i) <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t phi1 = phi_uint64(n / d1);
            result += phi1 * pow6((seq_t)d1);

            if (d1 != d2)
            {
                seq_t phi2 = phi_uint64(n / d2);
                result += phi2 * pow6((seq_t)d2);
            }
        }
    }
    return result;
}
