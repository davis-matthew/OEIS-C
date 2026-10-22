/*
 * A354501  The inverse Rijndael S-box used in the Advanced Encryption
 *          Standard (AES); inverse permutation of A354500.
 * https://oeis.org/A354501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = (((x^6 + x^3 + x) * p(x) + x^2 + 1)^254) mod (x^8 + x^4 +
 *            x^3 + x + 1) where p(x) is the polynomial whose coefficients are
 *            the binary digits of n, all arithmetic done in GF(2)[x]. The
 *            result r(x) is taken as an 8‑bit integer
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint8_t p = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (b & 1)
            p ^= a;
        uint8_t hi = a & 0x80;
        a <<= 1;
        if (hi)
            a ^= 0x1B; /* 0x1B == 0x11B without the top x^8 coefficient */
        b >>= 1;
    }
    return p;
}

static inline uint8_t gf_pow(uint8_t a, uint8_t exp)
{
    uint8_t result = 1; /* multiplicative identity in GF(2^8) */
    while (exp)
    {
        if (exp & 1)
            result = gf_mul(result, a);
        a = gf_mul(a, a);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint8_t q = 0;
    idx_t temp = n;
    unsigned i = 0;
    while (temp)
    {
        if (temp & 1ULL)
        {
            unsigned a1 = (i + 6) & 7;
            unsigned a2 = (i + 3) & 7;
            unsigned a3 = (i + 1) & 7;
            q ^= (uint8_t)(1u << a1);
            q ^= (uint8_t)(1u << a2);
            q ^= (uint8_t)(1u << a3);
        }
        temp >>= 1;
        ++i;
    }
    /* add x^2 + 1 */
    q ^= 1u | (1u << 2);
    uint8_t r = gf_pow(q, 254);
    return (seq_t)r;
}
