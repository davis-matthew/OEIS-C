/*
 * A276270  Number of solutions to the congruence x^3 + y^2 + 1 == 0 (mod n).
 * https://oeis.org/A276270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of pairs (x,y) with 0 <= x,y < n satisfying x^3 +
 *            y^2 + 1 ≡ 0 (mod n)
 * Range:     u8 n <= 161, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   count_y_mod_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_y_mod_2.h"

/* modular exponentiation for odd primes */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = (uint64_t)((__int128)res * base % mod);
        base = (uint64_t)((__int128)base * base % mod);
        exp >>= 1;
    }
    return res;
}

/* number of solutions y^2 ≡ c (mod p^e) for odd prime p */
static uint64_t count_y_mod_prime(uint64_t c, uint64_t pe, uint64_t p, unsigned e)
{
    /* exponent of p in c */
    unsigned v;
    if (c == 0) {
        v = e;                     /* treat as v >= e */
    } else {
        v = 0;
        uint64_t tmp = c;
        while (tmp % p == 0) {
            v++;
            tmp /= p;
        }
    }

    /* c divisible by p^e  */
    if (v >= e) {
        unsigned half = e >> 1;      /* floor(e/2) */
        uint64_t cnt = 1;
        for (unsigned i = 0; i < half; ++i)
            cnt *= p;
        return cnt;
    }

    /* odd exponent -> no solutions */
    if (v & 1)
        return 0ULL;

    /* v even, factor out p^v */
    uint64_t d = c;
    for (unsigned i = 0; i < v; ++i)
        d /= p;

    /* Legendre symbol: d mod p must be a quadratic residue */
    uint64_t leg = mod_pow64(d % p, (p - 1) / 2, p);
    if (leg != 1)
        return 0ULL;

    /* total solutions = 2 * p^{v/2} */
    unsigned t = v >> 1;          /* v = 2*t */
    uint64_t cnt = 2;
    for (unsigned i = 0; i < t; ++i)
        cnt *= p;
    return cnt;
}

/* compute number of solutions modulo a prime power p^e */
static seq_t count_prime_power(uint64_t p, unsigned e)
{
    /* p^e value */
    uint64_t pe = 1;
    for (unsigned i = 0; i < e; ++i)
        pe *= p;

    uint64_t total = 0;
    for (uint64_t x = 0; x < pe; ++x) {
        /* compute x^3 mod pe */
        uint64_t xx = (uint64_t)((__int128)x * x % pe);
        uint64_t x3 = (uint64_t)((__int128)xx * x % pe);

        /* c = -(x^3 + 1) mod pe */
        uint64_t c = (pe - ((x3 + 1) % pe)) % pe;

        uint64_t ycnt;
        if (p == 2)
            ycnt = count_y_mod_2(c, e);
        else
            ycnt = count_y_mod_prime(c, pe, p, e);

        total += ycnt;
    }
    return (seq_t)total;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but guard */

    seq_t product = 1;
    uint64_t nn = (uint64_t)n;

    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            unsigned e = 0;
            while (nn % p == 0) {
                nn /= p;
                e++;
            }
            product *= count_prime_power(p, e);
        }
    }

    if (nn > 1) /* remaining prime factor */
        product *= count_prime_power(nn, 1);

    return product;
}
