/*
 * A305816  a(n) = 1 if n is a prime whose binary expansion encodes a
 *          (0,1)-polynomial which is irreducible when factored over GF(2), 0
 *          otherwise.
 * https://oeis.org/A305816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime and its binary expansion encodes an *
 *            irreducible polynomial over GF(2); 0 otherwise. * *
 *            Characterset: a(1) = 0, a(2) = 1, a(3) = 1, a(4) = 0, a(5) = 0,
 *            …
 * Range:     u8 n <= 8101, u16 n <= 8101, u32 n <= 8101, u64 n <= 8101,
 *            u128 n <= 8101, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/*--------------------------------------------------------------*/
/*  Polynomial helper functions (polynomials represented by bit  */
/*  patterns of uint64_t, coefficients in GF(2))                 */
/*--------------------------------------------------------------*/
static unsigned int poly_deg(uint64_t a)
{
    unsigned int d = 0;
    while (a >> 1) { a >>= 1; ++d; }
    return d;
}

static uint64_t poly_mod_uint64(uint64_t a, uint64_t mod, unsigned int deg_mod)
{
    uint64_t limit = ((uint64_t)1 << deg_mod);
    while (a >= limit) {
        unsigned int shift = poly_deg(a) - deg_mod;
        a ^= mod << shift;
    }
    return a;
}

static uint64_t poly_mul_mod_uint64(uint64_t a, uint64_t b,
                                    uint64_t mod, unsigned int deg_mod)
{
    uint64_t prod = 0;
    while (a) {
        unsigned int i = 0;
        uint64_t t = a;
        while ((t & 1) == 0) { i++; t >>= 1; }
        prod ^= b << i;
        a &= a - 1;          /* remove lowest set bit */
    }
    return poly_mod_uint64(prod, mod, deg_mod);
}

static uint64_t poly_gcd_uint64(uint64_t a, uint64_t b)
{
    while (b) {
        unsigned int deg_b = poly_deg(b);
        uint64_t r = poly_mod_uint64(a, b, deg_b);
        a = b;
        b = r;
    }
    return a;
}

/*--------------------------------------------------------------*/
/*  Main function                                               */
/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 2 || !is_prime(n))
        return (seq_t)0;

    uint64_t f = (uint64_t)n;                  /* polynomial from bits of n */
    unsigned int d = poly_deg(f);              /* degree of polynomial */

    if (d <= 1)                                /* degree 0 or 1 polynomials are irreducible */
        return (seq_t)1;

    uint64_t g = 2ULL;                         /* polynomial 'x' (bit pattern 10) */
    unsigned int steps = d / 2;                /* loop up to floor(d/2) */

    for (unsigned int i = 0; i < steps; ++i) {
        g = poly_mul_mod_uint64(g, g, f, d);    /* g = g^2 mod f */
        uint64_t h = g ^ 2ULL;                 /* h = g - x  (bin. subtraction) */
        uint64_t gcd_val = poly_gcd_uint64(f, h);
        if (gcd_val != 1ULL)                  /* reducible */
            return (seq_t)0;
    }

    return (seq_t)1;
}
