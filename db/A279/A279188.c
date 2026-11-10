/*
 * A279188  Maximal entry in row c of triangle in A279185, where c =
 *          prime(n)^2 = A001248(n).
 * https://oeis.org/A279188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 2 modulo the odd part of p(p-1),
 *            where p = prime(n); a(n) = 1 if that odd part is 1. Reason: for
 *            c = p^2 the nonzero-period rows come from k coprime to p, whose
 *            orders mod p^2 are exactly the divisors of p(p-1), and ord'(2,
 *            m) is largest at the maximal odd part
 * Range:     u8 n <= 16, u16 n <= 106, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 70 terms through n = 197 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A279188_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)((unsigned __int128)x * y % m);
}

static uint64_t A279188_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A279188_mulmod(r, b, m);
        b = A279188_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A279188_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, p = 1;
    while (cnt < (uint64_t)n) {
        ++p;
        if (A279188_isprime(p)) ++cnt;
    }
    if (p == 2) return (seq_t)1;

    uint64_t q = p - 1;
    while (q % 2 == 0) q /= 2;
    uint64_t o = p * q;
    if (o <= 1) return (seq_t)1;

    /* phi(o) */
    uint64_t phi = o, t = o;
    for (uint64_t f = 2; f * f <= t; ++f) {
        if (t % f == 0) {
            while (t % f == 0) t /= f;
            phi = phi / f * (f - 1);
        }
    }
    if (t > 1) phi = phi / t * (t - 1);

    /* distinct prime factors of phi */
    uint64_t pf[64];
    int pc = 0;
    t = phi;
    for (uint64_t f = 2; f * f <= t; ++f) {
        if (t % f == 0) {
            pf[pc++] = f;
            while (t % f == 0) t /= f;
        }
    }
    if (t > 1) pf[pc++] = t;

    uint64_t ord = phi;
    for (int i = 0; i < pc; ++i) {
        while (ord % pf[i] == 0 && A279188_powmod(2, ord / pf[i], o) == 1)
            ord /= pf[i];
    }
    return (seq_t)ord;
}
