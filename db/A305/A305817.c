/*
 * A305817  Number of terms of A091206 <= n; Partial sums of A305816.
 * https://oeis.org/A305817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A305816(k), where A305816(k)=1 if k is prime
 *            and its binary expansion is an irreducible polynomial over
 *            GF(2), else 0
 * Range:     u8 n <= 9108, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 112 terms through n = 9395 (2026-11-08)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static unsigned int A305816_poly_deg(uint64_t a)
{
    unsigned int d = 0;
    while (a >> 1) { a >>= 1; ++d; }
    return d;
}

static uint64_t A305816_poly_mod(uint64_t a, uint64_t mod, unsigned int deg_mod)
{
    uint64_t limit = ((uint64_t)1 << deg_mod);
    while (a >= limit) {
        unsigned int shift = A305816_poly_deg(a) - deg_mod;
        a ^= mod << shift;
    }
    return a;
}

static uint64_t A305816_poly_mul_mod(uint64_t a, uint64_t b,
                                     uint64_t mod, unsigned int deg_mod)
{
    uint64_t prod = 0;
    while (a) {
        unsigned int i = 0;
        uint64_t t = a;
        while ((t & 1) == 0) { i++; t >>= 1; }
        prod ^= b << i;
        a &= a - 1;
    }
    return A305816_poly_mod(prod, mod, deg_mod);
}

static uint64_t A305816_poly_gcd(uint64_t a, uint64_t b)
{
    while (b) {
        unsigned int deg_b = A305816_poly_deg(b);
        uint64_t r = A305816_poly_mod(a, b, deg_b);
        a = b;
        b = r;
    }
    return a;
}

static int A305816_a(uint64_t n)
{
    if (n < 2 || !is_prime((idx_t)n))
        return 0;

    uint64_t f = n;
    unsigned int d = A305816_poly_deg(f);
    if (d <= 1)
        return 1;

    uint64_t g = 2ULL;
    unsigned int steps = d / 2;
    for (unsigned int i = 0; i < steps; ++i) {
        g = A305816_poly_mul_mod(g, g, f, d);
        uint64_t h = g ^ 2ULL;
        if (A305816_poly_gcd(f, h) != 1ULL)
            return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t last_n = 0;
    static uint64_t last_count = 0;

    uint64_t k, cnt;
    if (n >= last_n) {
        k = last_n;
        cnt = last_count;
    } else {
        k = 0;
        cnt = 0;
    }
    while (k < (uint64_t)n) {
        ++k;
        cnt += (uint64_t)A305816_a(k);
    }
    last_n = k;
    last_count = cnt;
    return (seq_t)cnt;
}
