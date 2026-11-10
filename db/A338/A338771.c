/*
 * A338771  Primes p such that A338770(p) is prime.
 * https://oeis.org/A338771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that A338770(p) = Sum_{r prime, q =
 *            p-2r prime} q is prime; computed by a sieve plus an NTT
 *            convolution (two moduli + CRT) giving A338770 for all odd p up
 *            to a growing limit
 * Range:     u8 n <= 8, u16 n <= 470, u32 n <= 3790, u64 n <= 3790,
 *            u128 n <= 3790, big any n
 * Validated: standard, pass, 70 terms through n = 3601 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

static uint64_t A338771_pw(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = r * b % m;
        b = b * b % m;
        e >>= 1;
    }
    return r;
}

/* SHARED_HELPER: in-place number-theoretic transform modulo an NTT prime */
static void A338771_ntt(uint32_t *a, size_t n, int inv, uint64_t m, uint64_t g)
{
    size_t i, j, k, len;
    for (i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    for (len = 2; len <= n; len <<= 1) {
        uint64_t w = A338771_pw(g, (m - 1) / len, m);
        if (inv) w = A338771_pw(w, m - 2, m);
        size_t half = len >> 1;
        for (i = 0; i < n; i += len) {
            uint64_t wn = 1;
            for (k = 0; k < half; k++) {
                uint64_t u = a[i + k];
                uint64_t v = (uint64_t)a[i + k + half] * wn % m;
                uint64_t s = u + v;
                if (s >= m) s -= m;
                a[i + k] = (uint32_t)s;
                a[i + k + half] = (uint32_t)(u >= v ? u - v : u + m - v);
                wn = wn * w % m;
            }
        }
    }
    if (inv) {
        uint64_t ni = A338771_pw((uint64_t)n, m - 2, m);
        for (i = 0; i < n; i++) a[i] = (uint32_t)((uint64_t)a[i] * ni % m);
    }
}

static void A338771_conv(const unsigned char *comp, size_t H, size_t N,
                         uint64_t m, uint64_t g, uint32_t *A, uint32_t *B)
{
    size_t i;
    for (i = 0; i < N; i++) { A[i] = 0; B[i] = 0; }
    for (i = 2; i < H; i++)
        if (!comp[i]) A[i] = 1;
    for (i = 1; i < H; i++) {
        size_t q = 2 * i + 1;
        if (!comp[q]) B[i] = (uint32_t)(q % m);
    }
    A338771_ntt(A, N, 0, m, g);
    A338771_ntt(B, N, 0, m, g);
    for (i = 0; i < N; i++) A[i] = (uint32_t)((uint64_t)A[i] * B[i] % m);
    A338771_ntt(A, N, 1, m, g);
}

static uint64_t *A338771_terms = NULL;
static size_t A338771_cnt = 0;
static uint64_t A338771_lim = 0;

static void A338771_build(uint64_t L)
{
    const uint64_t M1 = 998244353ULL, M2 = 469762049ULL;
    size_t i, j;
    unsigned char *comp = (unsigned char *)calloc((size_t)L + 2, 1);
    comp[0] = comp[1] = 1;
    for (i = 2; (uint64_t)i * i <= L; i++)
        if (!comp[i])
            for (j = i * i; j <= L; j += i) comp[j] = 1;

    size_t H = (size_t)((L - 1) / 2) + 1;   /* indices 0..H-1, 2*(H-1)+1 <= L */
    size_t N = 1;
    while (N < 2 * H) N <<= 1;

    uint32_t *A1 = (uint32_t *)malloc(sizeof(uint32_t) * N);
    uint32_t *A2 = (uint32_t *)malloc(sizeof(uint32_t) * N);
    uint32_t *B = (uint32_t *)malloc(sizeof(uint32_t) * N);
    A338771_conv(comp, H, N, M1, 3, A1, B);
    A338771_conv(comp, H, N, M2, 3, A2, B);
    free(B);

    uint64_t inv1 = A338771_pw(M1 % M2, M2 - 2, M2);
    size_t cap = 1024, tc = 0;
    uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    for (i = 1; i < H; i++) {
        uint64_t p = 2 * (uint64_t)i + 1;
        if (comp[p]) continue;
        uint64_t a1 = A1[i], a2 = A2[i];
        uint64_t d = (a2 + M2 - a1 % M2) % M2 * inv1 % M2;
        uint64_t s = a1 + M1 * d;
        if (s >= 2 && is_prime_u64(s)) {
            if (tc == cap) {
                cap *= 2;
                t = (uint64_t *)realloc(t, sizeof(uint64_t) * cap);
            }
            t[tc++] = p;
        }
    }
    free(A1);
    free(A2);
    free(comp);
    free(A338771_terms);
    A338771_terms = t;
    A338771_cnt = tc;
    A338771_lim = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (A338771_lim == 0) A338771_build((uint64_t)1 << 16);
    while (A338771_cnt < (size_t)n)
        A338771_build(A338771_lim * 2);
    return (seq_t)A338771_terms[n - 1];
}
