/*
 * A368908  a(n) is the number of integers x in range 0..n such that the k-th
 *          arithmetic derivative of A005940(1+x) is zero for some k >= 0,
 *          where A005940 is the Doudna sequence.
 * https://oeis.org/A368908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n-1) + A368907(n), where A368907(n) = 1 if
 *            the iterated arithmetic derivative of A005940(1+n) reaches 0,
 *            else 0. Computed by direct iteration; values divisible by p^p
 *            (4, 27, 3125, ...) never reach 0, since the derivative keeps
 *            them divisible by p^p
 * Range:     u8 n <= 1055, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 98 terms through n = 9499 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned __int128 A368908_u128;

static uint64_t A368908_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((A368908_u128)a * b) % m);
}

static uint64_t A368908_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A368908_mulmod(r, b, m);
        b = A368908_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A368908_isprime(uint64_t n) {
    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    if (n < 2) return false;
    for (i = 0; i < 12; ++i) {
        if (n % bases[i] == 0) return n == bases[i];
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (i = 0; i < 12; ++i) {
        uint64_t x = A368908_powmod(bases[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int r;
        bool comp = true;
        for (r = 1; r < s; ++r) {
            x = A368908_mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t A368908_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A368908_rho(uint64_t n) {
    uint64_t c;
    if ((n & 1) == 0) return 2;
    for (c = 1;; ++c) {
        uint64_t y = 2, x = 2, g = 1, q = 1, ys = 2;
        uint64_t r = 1, k, i, m = 128;
        do {
            x = y;
            for (i = 0; i < r; ++i) y = (A368908_mulmod(y, y, n) + c) % n;
            k = 0;
            do {
                ys = y;
                uint64_t lim = (m < r - k) ? m : r - k;
                for (i = 0; i < lim; ++i) {
                    y = (A368908_mulmod(y, y, n) + c) % n;
                    q = A368908_mulmod(q, x > y ? x - y : y - x, n);
                }
                g = A368908_gcd(q, n);
                k += m;
            } while (k < r && g == 1);
            r <<= 1;
        } while (g == 1);
        if (g == n) {
            do {
                ys = (A368908_mulmod(ys, ys, n) + c) % n;
                g = A368908_gcd(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

/* collect prime factors (with repetition) of n > 1 */
static void A368908_factor_rec(uint64_t n, uint64_t *out, int *cnt) {
    if (n == 1) return;
    if (A368908_isprime(n)) { out[(*cnt)++] = n; return; }
    uint64_t d = A368908_rho(n);
    A368908_factor_rec(d, out, cnt);
    A368908_factor_rec(n / d, out, cnt);
}

/* arithmetic derivative; returns false on overflow */
static bool A368908_deriv(uint64_t v, uint64_t *res) {
    uint64_t fac[128];
    int cnt = 0;
    uint64_t m = v, p;
    for (p = 2; p < 1000 && p * p <= m; p += (p == 2 ? 1 : 2)) {
        while (m % p == 0) { fac[cnt++] = p; m /= p; }
    }
    if (m > 1) A368908_factor_rec(m, fac, &cnt);
    A368908_u128 d = 0;
    int i;
    for (i = 0; i < cnt; ++i) d += (A368908_u128)(v / fac[i]);
    if (d > (A368908_u128)UINT64_MAX) return false;
    *res = (uint64_t)d;
    return true;
}

static bool A368908_reaches_zero(uint64_t v) {
    int iter;
    for (iter = 0; iter < 10000; ++iter) {
        if (v <= 1) return true;
        if (v % 4 == 0 || v % 27 == 0 || v % 3125 == 0 || v % 823543 == 0)
            return false;
        uint64_t d;
        if (!A368908_deriv(v, &d)) return false;
        if (d == v) return false;
        v = d;
    }
    return false;
}

static bool A368908_doudna_ok(uint64_t x) {
    static const uint64_t primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
        101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,
        193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,
        293,307,311
    };
    uint64_t m = x;
    int i = 0;
    A368908_u128 res = 1;
    while (m > 0) {
        if (m & 1) {
            res *= primes[i];
            if (res > (A368908_u128)UINT64_MAX) return false;
            m >>= 1;
        } else {
            m >>= 1;
            ++i;
            if (i >= 64) return false;
        }
    }
    return A368908_reaches_zero((uint64_t)res);
}

static uint32_t *A368908_cache = 0;
static size_t A368908_have = 0;
static size_t A368908_cap = 0;

seq_t a(idx_t n) {
    size_t need = (size_t)n + 1;
    if (need > A368908_cap) {
        size_t nc = A368908_cap ? A368908_cap : 1024;
        while (nc < need) nc *= 2;
        A368908_cache = (uint32_t *)realloc(A368908_cache, nc * sizeof(uint32_t));
        A368908_cap = nc;
    }
    while (A368908_have < need) {
        size_t x = A368908_have;
        uint32_t prev = x ? A368908_cache[x - 1] : 0;
        A368908_cache[x] = prev + (A368908_doudna_ok((uint64_t)x) ? 1u : 0u);
        ++A368908_have;
    }
    return (seq_t)A368908_cache[n];
}
