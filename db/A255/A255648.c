/*
 * A255648  Expansion of (a(q) + a(q^2) + a(q^3) + a(q^6) - 4) / 6 in powers
 *          of q where a() is a cubic AGM theta function.
 * https://oeis.org/A255648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with: * a(2^e) = 1 * a(3^e) = 2 (for e ≥
 *            1) * a(p^e) = e+1 if p ≡ 1 (mod 6) * a(p^e) = (1+(-1)^e)/2 if p
 *            ≡ 5 (mod 6) (i.e. 1 when e even, 0 when e odd)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset 1: n≥1 normally */

    uint64_t m = (uint64_t)n;      /* work on 64‑bit integer */
    seq_t result = 1;              /* multiplicative identity */

    /* factor out powers of 2 – they contribute 1 to the product */
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* trial division by odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            uint32_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }

            seq_t val;
            if (p == 3)
                val = 2;                         /* a(3^e) = 2 for all e≥1 */
            else if (p % 6 == 1)
                val = (seq_t)(e + 1);            /* e+1 */
            else                                 /* p % 6 == 5 */
            {
                if ((e & 1) == 0)                /* even exponent */
                    val = 1;
                else
                    return 0;                    /* odd exponent → zero */
            }
            result *= val;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1)
    {
        uint64_t p = m;
        if (p == 3)
            result *= 2;
        else if (p % 6 == 1)
            result *= (seq_t)2;                  /* exponent = 1 → 1+1 = 2 */
        else                                      /* p % 6 == 5, exponent = 1 odd */
            return 0;
    }

    return result;
}
