/*
 * A369959  Numbers k such that A003415(k) >= A276086(k) and gcd(k,
 *          A003415(k)) = gcd(k, A276086(k)), where A003415 is the arithmetic
 *          derivative, and A276086 is the primorial base exp-function.
 * https://oeis.org/A369959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. By the bound A003415(k) <= k*log2(k)/2, any term k has a
 *            small A276086(k). So, for each primorial-base length, list the
 *            digit vectors whose product Prod prime(i+1)^d_i fits under that
 *            bound, then check A003415(k) >= A276086(k) and gcd(k,
 *            A003415(k)) = gcd(k, A276086(k))
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 3195, u64 n <= 3195,
 *            u128 n <= 3195, big any n
 * Validated: standard, pass, 65 terms through n = 3167 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const uint64_t A369959_PR[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static uint64_t A369959_PRIM[17];

static uint64_t A369959_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A369959_mulm(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A369959_powm(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A369959_mulm(r, b, m);
        b = A369959_mulm(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A369959_isprime(uint64_t n) {
    static const uint64_t bs[12] = {2,3,5,7,11,13,17,19,23,29,31,37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++) {
        if (n == bs[i]) return true;
        if (n % bs[i] == 0) return false;
    }
    uint64_t d = n - 1; int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = A369959_powm(bs[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = A369959_mulm(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

static uint64_t A369959_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    for (uint64_t c = 1;; c++) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (A369959_mulm(x, x, n) + c) % n;
            y = (A369959_mulm(y, y, n) + c) % n;
            y = (A369959_mulm(y, y, n) + c) % n;
            d = A369959_gcd(x > y ? x - y : y - x, n);
        }
        if (d != n) return d;
    }
}

/* add k/q for each prime factor q (with multiplicity) of m */
static uint64_t A369959_dpart(uint64_t m, uint64_t k) {
    if (m == 1) return 0;
    if (A369959_isprime(m)) return k / m;
    uint64_t d = A369959_rho(m);
    return A369959_dpart(d, k) + A369959_dpart(m / d, k);
}

static uint64_t A369959_deriv(uint64_t k) {
    if (k < 2) return 0;
    uint64_t res = 0, m = k;
    for (uint64_t p = 2; p < 1000 && p * p <= m; p += (p == 2 ? 1 : 2)) {
        while (m % p == 0) { m /= p; res += k / p; }
    }
    return res + A369959_dpart(m, k);
}

static int A369959_bitlen(uint64_t x) {
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

static uint64_t *A369959_buf = NULL;
static size_t A369959_bcnt = 0, A369959_bcap = 0;

static void A369959_push(uint64_t k) {
    if (A369959_bcnt == A369959_bcap) {
        A369959_bcap = A369959_bcap ? A369959_bcap * 2 : 256;
        A369959_buf = (uint64_t *)realloc(A369959_buf, A369959_bcap * sizeof(uint64_t));
        if (!A369959_buf) abort();
    }
    A369959_buf[A369959_bcnt++] = k;
}

static void A369959_leaf(uint64_t k, uint64_t E) {
    if (k < 1) return;
    unsigned __int128 kb = (unsigned __int128)k * (unsigned)A369959_bitlen(k) / 2;
    if ((unsigned __int128)E > kb) return;
    uint64_t d = A369959_deriv(k);
    if (d < E) return;
    if (A369959_gcd(k, d) != A369959_gcd(k, E)) return;
    A369959_push(k);
}

static void A369959_dfs(int pos, uint64_t k, unsigned __int128 E, unsigned __int128 B) {
    if (pos < 0) { A369959_leaf(k, (uint64_t)E); return; }
    uint64_t p = A369959_PR[pos];
    unsigned __int128 e = E;
    for (uint64_t d = 0; d < p; d++) {
        if (e > B) break;
        A369959_dfs(pos - 1, k + d * A369959_PRIM[pos], e, B);
        e *= p;
    }
}

static int A369959_cmp(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static int level = 0;
    static bool init = false;
    if (!init) {
        A369959_PRIM[0] = 1;
        for (int i = 1; i <= 15; i++) A369959_PRIM[i] = A369959_PRIM[i - 1] * A369959_PR[i - 1];
        init = true;
    }
    while (cnt < (size_t)n && level <= 14) {
        int m = level;
        A369959_bcnt = 0;
        uint64_t p = A369959_PR[m];
        unsigned __int128 E = 1;
        for (uint64_t d = 1; d < p; d++) {
            E *= p;
            uint64_t kmax = (d + 1) * A369959_PRIM[m] - 1;
            unsigned __int128 B = (unsigned __int128)kmax * (unsigned)A369959_bitlen(kmax) / 2;
            if (E > B) continue;
            A369959_dfs(m - 1, d * A369959_PRIM[m], E, B);
        }
        if (A369959_bcnt > 1)
            qsort(A369959_buf, A369959_bcnt, sizeof(uint64_t), A369959_cmp);
        for (size_t i = 0; i < A369959_bcnt; i++) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = A369959_buf[i];
        }
        level++;
    }
    if ((size_t)n > cnt || n == 0) return (seq_t)0;
    return (seq_t)terms[(size_t)n - 1];
}
