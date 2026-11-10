/*
 * A343185  Numbers k such that 2*k is a multiple of A045917(k).
 * https://oeis.org/A343185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve search. a(n) = n-th k with A045917(k) > 0 and A045917(k) |
 *            2k, where A045917(k) = number of unordered pairs of primes p <=
 *            q with p + q = 2k. A045917(k) is found for every k up to a bound
 *            at once, by squaring the odd-prime indicator with an exact NTT
 *            (mod 998244353). The bound is multiplied...
 * Range:     u8 n <= 46, u16 n <= 325, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Validated: standard, pass, 83 terms through n = 572 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A343185_MOD 998244353u

static uint64_t *A343185_terms = NULL;
static size_t A343185_cnt = 0;
static uint64_t A343185_K = 0;

static uint32_t A343185_pw(uint32_t b, uint64_t e)
{
    uint64_t r = 1, x = b;
    while (e) {
        if (e & 1) r = r * x % A343185_MOD;
        x = x * x % A343185_MOD;
        e >>= 1;
    }
    return (uint32_t)r;
}

static void A343185_ntt(uint32_t *v, size_t L, int inv)
{
    size_t i, j, len;
    for (i = 1, j = 0; i < L; i++) {
        size_t bit = L >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = v[i]; v[i] = v[j]; v[j] = t; }
    }
    for (len = 2; len <= L; len <<= 1) {
        uint32_t w = A343185_pw(3, (A343185_MOD - 1) / len);
        if (inv) w = A343185_pw(w, A343185_MOD - 2);
        size_t half = len >> 1;
        for (i = 0; i < L; i += len) {
            uint64_t wn = 1;
            for (j = 0; j < half; j++) {
                uint32_t u = v[i + j];
                uint32_t t = (uint32_t)(wn * v[i + j + half] % A343185_MOD);
                uint32_t s = u + t;
                if (s >= A343185_MOD) s -= A343185_MOD;
                v[i + j] = s;
                v[i + j + half] = (u >= t) ? (u - t) : (u + A343185_MOD - t);
                wn = wn * w % A343185_MOD;
            }
        }
    }
    if (inv) {
        uint64_t il = A343185_pw((uint32_t)(L % A343185_MOD), A343185_MOD - 2);
        for (i = 0; i < L; i++) v[i] = (uint32_t)(v[i] * il % A343185_MOD);
    }
}

static void A343185_build(uint64_t K)
{
    uint64_t lim = 2 * K, i, j, k;
    unsigned char *comp = (unsigned char*)calloc((size_t)(lim + 1), 1);
    if (!comp) exit(EXIT_FAILURE);
    comp[0] = comp[1] = 1;
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;

    size_t L = 1;
    while (L < 2 * K) L <<= 1;
    uint32_t *f = (uint32_t*)calloc(L, sizeof(uint32_t));
    if (!f) exit(EXIT_FAILURE);
    /* odd prime p = 2i+1 -> index i ; p + q = 2k  <=>  i + j = k - 1 */
    for (i = 1; i < K; i++)
        if (!comp[2 * i + 1]) f[i] = 1;
    A343185_ntt(f, L, 0);
    for (i = 0; i < L; i++) f[i] = (uint32_t)((uint64_t)f[i] * f[i] % A343185_MOD);
    A343185_ntt(f, L, 1);

    free(A343185_terms);
    A343185_terms = (uint64_t*)malloc((size_t)K * sizeof(uint64_t));
    if (!A343185_terms) exit(EXIT_FAILURE);
    A343185_cnt = 0;
    for (k = 1; k <= K; k++) {
        uint64_t g;
        if (k == 1) g = 0;
        else if (k == 2) g = 1;
        else {
            uint64_t c = f[k - 1];
            if ((k & 1) && !comp[k]) c += 1;
            g = c / 2;
        }
        if (g > 0 && (2 * k) % g == 0)
            A343185_terms[A343185_cnt++] = k;
    }
    A343185_K = K;
    free(f);
    free(comp);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A343185_cnt < (size_t)n) {
        uint64_t K = A343185_K ? A343185_K * 4 : 4096;
        A343185_build(K);
    }
    return (seq_t)A343185_terms[n - 1];
}
