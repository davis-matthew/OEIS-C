/*
 * A328474  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A046523(A276156(i)) = A046523(A276156(j)) for all i, j.
 * https://oeis.org/A328474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence (order of first occurrence) of
 *            A046523(A276156(n)), i.e. the prime signature of A276156(n);
 *            computed by factoring (trial division + Pollard rho) and
 *            classifying signatures
 * Range:     u8 n <= 18511, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9016 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t A_mulmod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((unsigned __int128)a * b % m);
}
static uint64_t A_powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1; b %= m;
    while (e) { if (e & 1) r = A_mulmod(r, b, m); b = A_mulmod(b, b, m); e >>= 1; }
    return r;
}
static int A_isprime(uint64_t n) {
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return 0;
    for (int i = 0; i < 12; i++) { if (n % bs[i] == 0) return n == bs[i]; }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A_powmod(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int comp = 1;
        for (int r = 1; r < s; r++) {
            x = A_mulmod(x, x, n);
            if (x == n - 1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}
static uint64_t A_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}
static uint64_t A_rho(uint64_t n) {
    if ((n & 1) == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (A_mulmod(x, x, n) + c) % n;
            y = (A_mulmod(y, y, n) + c) % n;
            y = (A_mulmod(y, y, n) + c) % n;
            d = A_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}
static void A_split(uint64_t n, uint64_t *pf, int *cnt) {
    if (n == 1) return;
    if (A_isprime(n)) { pf[(*cnt)++] = n; return; }
    uint64_t d = A_rho(n);
    A_split(d, pf, cnt);
    A_split(n / d, pf, cnt);
}

/* A276156(n) passes 2^64 at n = 2^16 + 1 (bit 16 adds the primorial of the first 16
   primes, 3.3e19), so it is computed in 128 bits and factored with 128-bit
   Pollard rho when the cofactor is too big for A_split; mulmod128 splits a factor
   into 35-bit halves, exact for moduli below 2^81 (n < 2^19). */
typedef unsigned __int128 A_u128;

static A_u128 A_mulmod128(A_u128 a, A_u128 b, A_u128 m)
{
    A_u128 b1 = b >> 35, b0 = b & (((A_u128)1 << 35) - 1);
    A_u128 hi = (a * b1) % m;
    return ((hi << 35) % m + (a * b0) % m) % m;
}

static A_u128 A_powmod128(A_u128 b, A_u128 e, A_u128 m)
{
    A_u128 r = 1 % m;
    b %= m;
    for (; e; e >>= 1) {
        if (e & 1) r = A_mulmod128(r, b, m);
        b = A_mulmod128(b, b, m);
    }
    return r;
}

static int A_isprime128(A_u128 n)        /* exact below 3.3e24 (13 bases) */
{
    static const unsigned P[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    if (n < 2) return 0;
    for (int i = 0; i < 13; ++i)
        if (n % P[i] == 0) return n == P[i];
    A_u128 d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (int i = 0; i < 13; ++i) {
        A_u128 x = A_powmod128(P[i], d, n);
        if (x == 1 || x == n - 1) continue;
        int ok = 0;
        for (int r = 1; r < s && !ok; ++r) {
            x = A_mulmod128(x, x, n);
            ok = x == n - 1;
        }
        if (!ok) return 0;
    }
    return 1;
}

static A_u128 A_rho128(A_u128 n)
{
    if (!(n & 1)) return 2;
    for (A_u128 c = 1;; ++c) {
        A_u128 x = 2, y = 2, g = 1;
        while (g == 1) {
            x = (A_mulmod128(x, x, n) + c) % n;
            y = (A_mulmod128(y, y, n) + c) % n;
            y = (A_mulmod128(y, y, n) + c) % n;
            A_u128 d = x > y ? x - y : y - x, a = d, b = n;
            while (b) { A_u128 t = a % b; a = b; b = t; }
            g = a;
        }
        if (g != n) return g;
    }
}

/* the prime factors of n with multiplicity, appended to pf (each fits 128 bits) */
static void A_split128(A_u128 n, A_u128 *pf, int *cnt)
{
    if (n == 1) return;
    if (n <= UINT64_MAX) {
        uint64_t t[80];
        int k = 0;
        A_split((uint64_t)n, t, &k);
        for (int i = 0; i < k; ++i) pf[(*cnt)++] = t[i];
        return;
    }
    if (n >> 81) abort();                /* beyond the exact range of A_mulmod128 */
    if (A_isprime128(n)) { pf[(*cnt)++] = n; return; }
    A_u128 d = A_rho128(n);
    A_split128(d, pf, cnt);
    A_split128(n / d, pf, cnt);
}

static void A_signature(A_u128 m, uint8_t sig[16]) {
    A_u128 pf[80]; int cnt = 0;
    memset(sig, 0, 16);
    for (uint64_t d = 2; d < 1000 && (A_u128)d * d <= m; d++) {
        while (m % d == 0) { pf[cnt++] = d; m /= d; }
    }
    if (m > 1) A_split128(m, pf, &cnt);
    /* sort primes */
    for (int i = 1; i < cnt; i++) {
        A_u128 v = pf[i]; int j = i - 1;
        while (j >= 0 && pf[j] > v) { pf[j + 1] = pf[j]; j--; }
        pf[j + 1] = v;
    }
    uint8_t ex[40]; int ne = 0;
    for (int i = 0; i < cnt;) {
        int j = i;
        while (j < cnt && pf[j] == pf[i]) j++;
        ex[ne++] = (uint8_t)(j - i);
        i = j;
    }
    /* sort descending */
    for (int i = 1; i < ne; i++) {
        uint8_t v = ex[i]; int j = i - 1;
        while (j >= 0 && ex[j] < v) { ex[j + 1] = ex[j]; j--; }
        ex[j + 1] = v;
    }
    for (int i = 0; i < ne && i < 16; i++) sig[i] = ex[i];
}

static A_u128 A_prim_sum(uint64_t n) {
    static const unsigned primes[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
                                        59,61,67,71,73,79,83,89};
    A_u128 prim = 1, res = 0;
    int pos = 0;
    while (n) {
        if (n & 1) res += prim;
        n >>= 1;
        if (pos < 24) prim *= primes[pos];
        pos++;
    }
    return res;
}

seq_t a(idx_t n) {
    static unsigned *vals = 0;
    static size_t cap = 0, done = 0;
    static uint8_t *cls = 0;
    static size_t ncls = 0, clscap = 0;
    if (n == 0) return (seq_t)0;
    if ((size_t)n > cap) {
        size_t nc = cap ? cap : 1024;
        while (nc < (size_t)n) nc *= 2;
        vals = (unsigned *)realloc(vals, nc * sizeof(unsigned));
        cap = nc;
    }
    while (done < (size_t)n) {
        size_t k = done + 1;
        uint8_t sig[16];
        A_signature(A_prim_sum((uint64_t)k), sig);
        size_t id = 0;
        for (size_t c = 0; c < ncls; c++) {
            if (memcmp(cls + c * 16, sig, 16) == 0) { id = c + 1; break; }
        }
        if (!id) {
            if (ncls == clscap) {
                clscap = clscap ? clscap * 2 : 64;
                cls = (uint8_t *)realloc(cls, clscap * 16);
            }
            memcpy(cls + ncls * 16, sig, 16);
            ncls++;
            id = ncls;
        }
        vals[k - 1] = (unsigned)id;
        done = k;
    }
    return (seq_t)vals[n - 1];
}
