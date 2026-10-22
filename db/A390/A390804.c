/*
 * A390804  The sum of the integers k from 1 to n such that gcd(n, k) is a
 *          power of 2.
 * https://oeis.org/A390804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n, gcd(k,n) is a power of 2} k a(1) = 1 for n =
 *            2^e, e≥1: a(n) = 2^(e-1) * (2^e + 1) otherwise: a(n) = φ(2n) * n
 *            / 2 (φ = Euler's totient)
 * Range:     u8 n <= 25, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

static seq_t phi_uint64(uint64_t m)
{
    seq_t result = (seq_t)m;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0) temp /= p;
            result = result - result / (seq_t)p;
        }
    }
    if (temp > 1)
        result = result - result / (seq_t)temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* power-of-two test (n > 1) */
    if ((n & (n - 1)) == 0)
    {
        /* compute e such that n = 2^e (e ≥ 1) */
        int e = 0;
        uint64_t t = n;
        while (t > 1)
        {
            t >>= 1;
            ++e;
        }
        seq_t twoprev = pow2((idx_t)(e - 1));
        seq_t twoe     = pow2((idx_t)e);
        return twoprev * (twoe + (seq_t)1);
    }

    /* general case */
    uint64_t m = n << 1;               /* 2n fits in uint64_t for n ≤ 2^63-1 */
    seq_t phi2n = phi_uint64(m);
    return phi2n * (seq_t)n / 2;
}
