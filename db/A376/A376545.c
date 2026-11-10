/*
 * A376545  Least prime factor of A252043(n) for n > 1 with a(1) = 1.
 * https://oeis.org/A376545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A020639(A252043(n)), a(1) = 1. Computed by search: trial
 *            division of the Champernowne prefix (reduced digit-chunk by
 *            digit-chunk, so it never overflows), then Miller-Rabin and
 *            Pollard-Brent rho on the remaining cofactor
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 13, u64 n <= 23, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-10)
 * Helpers:   gcd_u128, mulmod_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mulmod_u128.h"
#include "../utils/gcd_u128.h"

typedef unsigned __int128 c_u128;

static const unsigned champ_mr_bases[12] = {2,3,5,7,11,13,17,19,23,29,31,37};

/* decimal digits of the first n digits of the Champernowne constant */
static uint8_t *champ_digits(idx_t n)
{
    uint8_t *d = (uint8_t *)malloc((size_t)n + 1);
    idx_t pos = 0;
    for (idx_t k = 1; pos < n; ++k) {
        uint8_t buf[24];
        int len = 0;
        idx_t t = k;
        while (t > 0) { buf[len++] = (uint8_t)(t % 10); t /= 10; }
        for (int i = len - 1; i >= 0 && pos < n; --i) d[pos++] = buf[i];
    }
    return d;
}

static uint64_t champ_mod(const uint64_t *limb, const uint64_t *mul, size_t L, uint64_t p)
{
    uint64_t r = 0;
    for (size_t j = 0; j < L; ++j) r = (r * mul[j] + limb[j]) % p;
    return r;
}

static bool champ_is_big(void)
{
    seq_t x = 1;
    for (int i = 0; i < 140; ++i) x *= 2;
    return x != 0;
}

/* ---------- arbitrary-precision path (seq_t) ---------- */
static seq_t sq_absdiff(seq_t x, seq_t y) { return x > y ? x - y : y - x; }

static seq_t sq_gcd(seq_t x, seq_t y)
{
    while (y != 0) { seq_t t = x % y; x = y; y = t; }
    return x;
}

static seq_t sq_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1;
    b %= m;
    while (e > 0) {
        if (e % 2 == 1) r = r * b % m;
        b = b * b % m;
        e /= 2;
    }
    return r;
}

static bool sq_isprime(seq_t m)
{
    if (m < 2) return false;
    for (int i = 0; i < 12; ++i) {
        seq_t p = (seq_t)champ_mr_bases[i];
        if (m == p) return true;
        if (m % p == 0) return false;
    }
    seq_t d = m - 1;
    int s = 0;
    while (d % 2 == 0) { d /= 2; ++s; }
    for (int i = 0; i < 12; ++i) {
        seq_t x = sq_powmod((seq_t)champ_mr_bases[i], d, m);
        if (x == 1 || x == m - 1) continue;
        bool ok = false;
        for (int j = 1; j < s; ++j) {
            x = x * x % m;
            if (x == m - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static seq_t sq_rho(seq_t m)
{
    for (unsigned c = 1; c < 20; ++c) {
        seq_t y = 2, x = 2, ys = 2, q = 1, g = 1;
        uint64_t r = 1;
        while (g == 1) {
            x = y;
            for (uint64_t i = 0; i < r; ++i) y = (y * y + c) % m;
            uint64_t k = 0;
            while (k < r && g == 1) {
                ys = y;
                uint64_t lim = (r - k < 128) ? r - k : 128;
                for (uint64_t i = 0; i < lim; ++i) {
                    y = (y * y + c) % m;
                    q = q * sq_absdiff(x, y) % m;
                }
                g = sq_gcd(q, m);
                k += 128;
            }
            r *= 2;
            if (r > ((uint64_t)1 << 22)) break;
        }
        if (g == 1) return m;
        if (g == m) {
            do {
                ys = (ys * ys + c) % m;
                g = sq_gcd(sq_absdiff(x, ys), m);
            } while (g == 1);
        }
        if (g != m) return g;
    }
    return m;
}

static seq_t sq_smallest(seq_t m)
{
    if (m <= 1) return m;
    if (sq_isprime(m)) return m;
    seq_t d = sq_rho(m);
    if (d == m) return m;
    seq_t p = sq_smallest(d);
    seq_t q = sq_smallest(m / d);
    return p < q ? p : q;
}

/* ---------- 128-bit path ---------- */
static c_u128 u_absdiff(c_u128 x, c_u128 y) { return x > y ? x - y : y - x; }

static c_u128 u_powmod(c_u128 b, c_u128 e, c_u128 m)
{
    c_u128 r = 1;
    b %= m;
    while (e > 0) {
        if (e & 1) r = mulmod_u128(r, b, m);
        b = mulmod_u128(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool u_isprime(c_u128 m)
{
    if (m < 2) return false;
    for (int i = 0; i < 12; ++i) {
        c_u128 p = champ_mr_bases[i];
        if (m == p) return true;
        if (m % p == 0) return false;
    }
    c_u128 d = m - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (int i = 0; i < 12; ++i) {
        c_u128 x = u_powmod(champ_mr_bases[i], d, m);
        if (x == 1 || x == m - 1) continue;
        bool ok = false;
        for (int j = 1; j < s; ++j) {
            x = mulmod_u128(x, x, m);
            if (x == m - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static c_u128 u_rho(c_u128 m)
{
    for (unsigned c = 1; c < 20; ++c) {
        c_u128 y = 2, x = 2, ys = 2, q = 1, g = 1;
        uint64_t r = 1;
        while (g == 1) {
            x = y;
            for (uint64_t i = 0; i < r; ++i) y = (mulmod_u128(y, y, m) + c) % m;
            uint64_t k = 0;
            while (k < r && g == 1) {
                ys = y;
                uint64_t lim = (r - k < 128) ? r - k : 128;
                for (uint64_t i = 0; i < lim; ++i) {
                    y = (mulmod_u128(y, y, m) + c) % m;
                    q = mulmod_u128(q, u_absdiff(x, y), m);
                }
                g = gcd_u128(q, m);
                k += 128;
            }
            r *= 2;
            if (r > ((uint64_t)1 << 22)) break;
        }
        if (g == 1) return m;
        if (g == m) {
            do {
                ys = (mulmod_u128(ys, ys, m) + c) % m;
                g = gcd_u128(u_absdiff(x, ys), m);
            } while (g == 1);
        }
        if (g != m) return g;
    }
    return m;
}

static c_u128 u_smallest(c_u128 m)
{
    if (m <= 1) return m;
    if (u_isprime(m)) return m;
    c_u128 d = u_rho(m);
    if (d == m) return m;
    c_u128 p = u_smallest(d);
    c_u128 q = u_smallest(m / d);
    return p < q ? p : q;
}

static seq_t u_to_seq(c_u128 v)
{
    seq_t r = (seq_t)(uint64_t)(v >> 64);
    r *= (seq_t)4294967296ULL;
    r *= (seq_t)4294967296ULL;
    r += (seq_t)(uint64_t)v;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint8_t *dig = champ_digits(n);

    size_t L = (size_t)((n + 8) / 9);
    uint64_t *limb = (uint64_t *)malloc(L * sizeof(uint64_t));
    uint64_t *mul = (uint64_t *)malloc(L * sizeof(uint64_t));
    idx_t first = n - (idx_t)9 * (idx_t)(L - 1);
    idx_t pos = 0;
    for (size_t j = 0; j < L; ++j) {
        idx_t len = (j == 0) ? first : 9;
        uint64_t v = 0, m = 1;
        for (idx_t t = 0; t < len; ++t) { v = v * 10 + dig[pos++]; m *= 10; }
        limb[j] = v;
        mul[j] = m;
    }

    bool fits64 = (n <= 19);
    uint64_t N64 = 0;
    if (fits64)
        for (idx_t i = 0; i < n; ++i) N64 = N64 * 10 + dig[i];

    seq_t result = 0;
    bool done = false;
    const uint64_t B = 2000000;

    if (champ_mod(limb, mul, L, 2) == 0) { result = (seq_t)2; done = true; }
    for (uint64_t i = 3; !done && i <= B; i += 2) {
        if (fits64 && i > N64 / i) { result = (seq_t)N64; done = true; break; }
        if (champ_mod(limb, mul, L, i) == 0) { result = (seq_t)i; done = true; break; }
    }

    if (!done) {
        if (champ_is_big()) {
            seq_t N = 0;
            for (idx_t i = 0; i < n; ++i) N = N * 10 + (seq_t)dig[i];
            result = sq_smallest(N);
        } else if (n <= 38) {
            c_u128 N = 0;
            for (idx_t i = 0; i < n; ++i) N = N * 10 + dig[i];
            result = u_to_seq(u_smallest(N));
        } else {
            result = 0; /* least prime factor exceeds any value representable here */
        }
    }

    free(dig);
    free(limb);
    free(mul);
    return result;
}
