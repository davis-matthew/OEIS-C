/*
 * A294881  Number of proper divisors of n that are irreducible when their
 *          binary expansion is interpreted as polynomial over GF(2).
 * https://oeis.org/A294881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of proper divisors d of n such that the binary
 *            representation of d, interpreted as a polynomial over GF(2), is
 *            irreducible
 * Range:     u8 n <= 9975, u16 n <= 9975, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ----------------- polynomial helpers over GF(2) ----------------- */

static int poly_deg(uint32_t p)
{
    int d = -1;
    while (p) {
        p >>= 1;
        ++d;
    }
    return d;
}

static uint32_t poly_mod(uint32_t a, uint32_t b)
{
    int db = poly_deg(b);
    if (db < 0) return a; /* mod by 0 undefined, but never used */
    while (poly_deg(a) >= db) {
        int shift = poly_deg(a) - db;
        a ^= (b << shift);
    }
    return a;
}

static uint32_t poly_mul_mod(uint32_t a, uint32_t b, uint32_t mod)
{
    uint32_t prod = 0;
    while (a) {
        if (a & 1U) prod ^= b;
        a >>= 1U;
        b <<= 1U;
    }
    return poly_mod(prod, mod);
}

static uint32_t poly_gcd(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t r = poly_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

/* Test whether integer d (>=2) has an irreducible binary polynomial. */
static bool is_irreducible(uint32_t d)
{
    if (d <= 1) return false;
    int deg = poly_deg(d);

    /* derivative in GF(2): keep only odd exponents */
    uint32_t der = 0;
    for (int i = 1; i <= deg; ++i) {
        if ((d >> i) & 1U) {
            if (i & 1) der ^= 1U << (i - 1);
        }
    }
    if (der == 0) return false;                 /* all exponents even -> reducible */
    if (poly_gcd(d, der) > 1) return false;      /* repeated root -> reducible */

    /* Irreducibility test */
    uint32_t xpow = 2; /* x */
    for (int i = 1; i <= deg / 2; ++i) {
        xpow = poly_mul_mod(xpow, xpow, d);       /* x^(2^i) mod d */
        uint32_t h_minus_x = xpow ^ 2;            /* h - x */
        if (poly_gcd(h_minus_x, d) > 1) return false;
    }
    return true;
}

/* ----------------- precomputation of irreducible flags ----------------- */
#define MAXN 10000
static bool irred[MAXN + 1];
static bool irred_init = false;

static void init_irred(void)
{
    if (irred_init) return;
    irred_init = true;
    irred[0] = irred[1] = false;
    for (int d = 2; d <= MAXN; ++d) {
        irred[d] = is_irreducible((uint32_t)d);
    }
}

/* ----------------- the requested sequence function ----------------- */
seq_t a(idx_t n)
{
    init_irred();                            /* ensure irreducible flags ready */

    if (n <= 1) return 0;                    /* a(1) = 0, a(0) irrelevant */

    int nval = (int)n;                       /* n <= 10000 by problem statement */
    seq_t count = 0;

    for (int i = 1; (uint64_t)i * (uint64_t)i <= (uint64_t)nval; ++i) {
        if (nval % i == 0) {
            int d1 = i;
            int d2 = nval / i;
            if (d1 < nval && irred[d1]) ++count;
            if (d2 < nval && d2 != d1 && irred[d2]) ++count;
        }
    }
    return count;
}
