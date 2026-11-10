/*
 * A279187  Maximal entry in row c of A279185, where c = n-th composite number
 *          A002808(n).
 * https://oeis.org/A279187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A256608(c), c = A002808(n), i.e. the multiplicative order
 *            of 2 modulo the odd part of the Carmichael lambda(c). This is
 *            the LCM variant, which the OEIS comment says equals a(n) for n
 *            <= 1100. Found by searching for the n-th composite
 * Range:     u8 n <= 979, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 996 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a279187_is_prime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t i = 3; i * i <= m; i += 2)
        if (m % i == 0) return false;
    return true;
}

static uint64_t a279187_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t target = (uint64_t)n, count = 0, c = 3;
    while (count < target) {
        ++c;
        if (!a279187_is_prime(c)) ++count;
    }

    /* Carmichael lambda(c) */
    uint64_t lam = 1, t = c;
    for (uint64_t p = 2; p * p <= t || t > 1; ++p) {
        if (p * p > t) p = t;
        if (t % p != 0) continue;
        unsigned e = 0;
        uint64_t pe = 1;
        while (t % p == 0) { t /= p; ++e; pe *= p; }
        uint64_t li;
        if (p == 2) {
            if (e == 1) li = 1;
            else if (e == 2) li = 2;
            else li = pe / 4;
        } else {
            li = pe / p * (p - 1);
        }
        uint64_t g = a279187_gcd(lam, li);
        lam = lam / g * li;
    }

    while ((lam & 1) == 0) lam >>= 1;
    if (lam == 1) return (seq_t)1;

    uint64_t pw = 2 % lam, k = 1;
    while (pw != 1) {
        pw = (pw * 2) % lam;
        ++k;
    }
    return (seq_t)k;
}
