/*
 * A282623  Number of independent cycles of the multiplicative group of
 *          integers modulo A033949(n).
 * https://oeis.org/A282623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of maximal cyclic subgroups of the group (Z/mZ)*,
 *            where m = A033949(n); computed by brute force over the elements
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool A282623_isOddPP(uint64_t n) {
    if (n <= 1) return false;
    uint64_t temp = n, p = 0;
    for (uint64_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) { p = i; break; }
    }
    if (p == 0) return n != 2;
    if (p == 2) return false;
    while (temp % p == 0) temp /= p;
    return temp == 1;
}

static uint64_t A282623_A033949(uint64_t target) {
    uint64_t count = 0, c = 2;
    while (count < target) {
        ++c;
        if (c == 4) continue;
        if (A282623_isOddPP(c)) continue;
        if ((c & 3ULL) == 2 && A282623_isOddPP(c >> 1)) continue;
        ++count;
    }
    return c;
}

static uint64_t A282623_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A282623_phi(uint64_t n) {
    uint64_t r = n;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            r -= r / p;
        }
    }
    if (n > 1) r -= r / n;
    return r;
}

static uint64_t A282623_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n) {
    uint64_t m = A282623_A033949((uint64_t)n);
    unsigned char *nonmax = (unsigned char *)calloc(m + 1, 1);
    uint64_t *ord = (uint64_t *)calloc(m + 1, sizeof(uint64_t));
    uint64_t *cnt = (uint64_t *)calloc(m + 1, sizeof(uint64_t));
    for (uint64_t b = 1; b < m; ++b) {
        if (A282623_gcd(b, m) != 1) continue;
        uint64_t x = b, d = 1;
        while (x != 1) { x = x * b % m; ++d; }
        ord[b] = d;
        uint64_t t = d;
        for (uint64_t q = 2; q * q <= t; ++q) {
            if (t % q == 0) {
                while (t % q == 0) t /= q;
                nonmax[A282623_powmod(b, q, m)] = 1;
            }
        }
        if (t > 1) nonmax[A282623_powmod(b, t, m)] = 1;
    }
    for (uint64_t b = 1; b < m; ++b) {
        if (ord[b] && !nonmax[b]) cnt[ord[b]]++;
    }
    uint64_t total = 0;
    for (uint64_t d = 1; d < m; ++d) {
        if (cnt[d]) total += cnt[d] / A282623_phi(d);
    }
    free(nonmax); free(ord); free(cnt);
    return (seq_t)total;
}
