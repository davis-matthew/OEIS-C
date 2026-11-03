/*
 * A354099  The 3-adic valuation of Euler totient function phi.
 * https://oeis.org/A354099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v₃(φ(n))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9233 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned int v3_u64(uint64_t x)
{
    unsigned int c = 0;
    while (x % 3 == 0)
    {
        x /= 3;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                   /* undefined for n = 0, but return a safe value */

    uint64_t m = n;
    seq_t res = 0;

    /* factor 2 – it never contributes to v₃(φ) */
    while ((m & 1) == 0)
        m >>= 1;

    /* trial division by odd numbers up to sqrt(m) */
    for (uint64_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            unsigned int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            unsigned int v3p1 = v3_u64(p - 1);
            seq_t val = (seq_t)v3p1;
            if (p == 3)
                val += (seq_t)(e - 1);    /* v₃(p) = 1 for p = 3 */
            res += val;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1)
    {
        uint64_t p = m;   /* exponent e = 1 */
        unsigned int v3p1 = v3_u64(p - 1);
        seq_t val = (seq_t)v3p1;   /* p == 3 would add e-1 = 0 */
        res += val;
    }

    return res;
}
