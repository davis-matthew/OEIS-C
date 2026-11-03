/*
 * A354500  The Rijndael S-box used in the Advanced Encryption Standard (AES).
 * https://oeis.org/A354500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = ivgenpoly(((x^4+x^3+x^2+x+1)*(genpoly(n)^254 mod
 *            (x^8+x^4+x^3+x+1)) + x^6 + x^5 + x + 1) mod (x^8+1))
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Validated: full, pass, 256 terms through n = 255 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two elements of GF(2^8) using the AES irreducible polynomial
   x^8 + x^4 + x^3 + x + 1  (0x11B).  */
static uint8_t gf_mul(uint8_t a, uint8_t b)
{
    uint16_t res = 0;          /* wide enough to hold intermediate values */
    uint16_t aa  = a;          /* use a 16‑bit accumulator to detect high bits */

    while (b) {
        if (b & 1)
            res ^= aa;
        aa <<= 1;
        if (aa & 0x100)        /* degree == 8 ⇒ reduce modulo 0x11B */
            aa ^= 0x11B;
        b >>= 1;
    }
    return (uint8_t)res;
}

/* Power of a field element: base^exp mod the field, where exp is small
   (here exp = 254).  */
static uint8_t gf_pow(uint8_t base, uint8_t exp)
{
    uint8_t r = 1;
    while (exp) {
        if (exp & 1)
            r = gf_mul(r, base);
        base = gf_mul(base, base);
        exp >>= 1;
    }
    return r;
}

/* Reduce a binary polynomial of degree < 64 modulo the AES irreducible
   polynomial (0x11B), obtaining an element of GF(2^8). */
static uint8_t poly_mod(uint64_t p)
{
    while (p >> 8) {                     /* degree >= 8 → still reducible */
        unsigned int shift = 63 - __builtin_clzll(p); /* highest 1‑bit pos */
        p ^= (uint64_t)0x11B << (shift - 8);          /* cancel leading term */
    }
    return (uint8_t)p;
}

/* 8‑bit left rotation. The second argument is taken modulo 8. */
static inline uint8_t rotl8(uint8_t v, unsigned int s)
{
    s &= 7;
    return (v << s) | (v >> (8 - s));
}

/* Compute the A354500 term a(n).  */
seq_t a(idx_t n)
{
    /* generator polynomial of index n, reduced to GF(2^8) */
    uint8_t p = poly_mod((uint64_t)n);

    /* p(x)^254 mod the AES field: multiplicative inverse of p */
    uint8_t q = gf_pow(p, 254);

    /* Affine transformation */
    uint8_t s = q ^ rotl8(q, 1) ^ rotl8(q, 2)
                ^ rotl8(q, 3) ^ rotl8(q, 4) ^ 0x63;

    return (seq_t)s;
}
