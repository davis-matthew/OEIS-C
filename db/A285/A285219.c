/*
 * A285219  Indices of primes in A003105.
 * https://oeis.org/A285219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th m >= 0 with A003105(m) prime, where Sum
 *            A003105(m) x^m = E(2)E(3)/(E(1)E(6)) and E(q) = Prod_{k>=1}
 *            (1-x^(qk)); coefficients computed exactly as multi-limb integers
 *            (mod 2^(64W), W bounding their size), primality by trial
 *            division + Miller-Rabin
 * Range:     u8 n <= 32, u16 n <= 305, u32 n <= 373, u64 n <= 373,
 *            u128 n <= 373, big any n
 * Validated: standard, pass, 72 terms through n = 365 (2026-11-08)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

typedef unsigned __int128 A285219_u128;
#define A285219_MAXK 192

/* limbs (64-bit) sufficient to hold A003105(i) */
static size_t A285219_wl(idx_t i) {
    uint64_t bits = (454 * (isqrt_u64(2 * (uint64_t)i) + 1)) / 300 + 64;
    return (size_t)(bits / 64 + 1);
}

static void A285219_add(uint64_t *d, const uint64_t *s, size_t w) {
    uint64_t c = 0;
    for (size_t i = 0; i < w; i++) {
        A285219_u128 z = (A285219_u128)d[i] + s[i] + c;
        d[i] = (uint64_t)z; c = (uint64_t)(z >> 64);
    }
}

static uint64_t A285219_sub(uint64_t *d, const uint64_t *s, size_t w) {
    uint64_t b = 0;
    for (size_t i = 0; i < w; i++) {
        A285219_u128 z = (A285219_u128)d[i] - s[i] - b;
        d[i] = (uint64_t)z; b = ((uint64_t)(z >> 64)) ? 1 : 0;
    }
    return b;
}

static int A285219_cmp(const uint64_t *a, const uint64_t *b, size_t k) {
    for (size_t i = k; i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

/* generalized pentagonal numbers <= maxg with signs (Euler) */
static size_t A285219_pent(idx_t maxg, idx_t **g, int **s) {
    size_t cap = 2 * ((size_t)isqrt_u64(maxg) + 3) + 2, c = 0;
    *g = (idx_t *)malloc(cap * sizeof(idx_t));
    *s = (int *)malloc(cap * sizeof(int));
    (*g)[c] = 0; (*s)[c] = 1; c++;
    for (idx_t j = 1;; j++) {
        idx_t g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
        int sg = (j & 1) ? -1 : 1;
        if (g1 > maxg) break;
        (*g)[c] = g1; (*s)[c] = sg; c++;
        if (g2 > maxg) break;
        (*g)[c] = g2; (*s)[c] = sg; c++;
    }
    return c;
}

static void A285219_series(idx_t L, size_t Wd, uint64_t *U) {
    int64_t *P = (int64_t *)calloc((size_t)L + 1, sizeof(int64_t));
    idx_t *g2, *g3, *g6, *g1; int *s2, *s3, *s6, *s1;
    size_t n2 = A285219_pent(L / 2, &g2, &s2), n3 = A285219_pent(L / 3, &g3, &s3);
    size_t n6 = A285219_pent(L / 6, &g6, &s6), n1 = A285219_pent(L, &g1, &s1);
    for (size_t x = 0; x < n2; x++)
        for (size_t y = 0; y < n3; y++) {
            idx_t e = 2 * g2[x] + 3 * g3[y];
            if (e > L) break;
            P[e] += s2[x] * s3[y];
        }
    for (idx_t i = 0; i <= L; i++) {
        uint64_t *d = U + (size_t)i * Wd;
        uint64_t fill = P[i] < 0 ? ~(uint64_t)0 : 0;
        d[0] = (uint64_t)P[i];
        for (size_t t = 1; t < Wd; t++) d[t] = fill;
        for (size_t t = 1; t < n6; t++) {
            idx_t e = 6 * g6[t];
            if (e > i) break;
            const uint64_t *src = U + (size_t)(i - e) * Wd;
            if (s6[t] < 0) A285219_add(d, src, Wd); else A285219_sub(d, src, Wd);
        }
    }
    for (idx_t i = 0; i <= L; i++) {
        uint64_t *d = U + (size_t)i * Wd;
        size_t w = A285219_wl(i);
        if (w > Wd) w = Wd;
        for (size_t t = 1; t < n1; t++) {
            idx_t e = g1[t];
            if (e > i) break;
            const uint64_t *src = U + (size_t)(i - e) * Wd;
            if (s1[t] < 0) A285219_add(d, src, w); else A285219_sub(d, src, w);
        }
        for (size_t t = w; t < Wd; t++) d[t] = 0;
    }
    free(P); free(g1); free(s1); free(g2); free(s2); free(g3); free(s3); free(g6); free(s6);
}

static void A285219_mmul(uint64_t *r, const uint64_t *a, const uint64_t *b,
                         const uint64_t *N, size_t k, uint64_t ninv) {
    uint64_t t[A285219_MAXK + 2];
    memset(t, 0, (k + 2) * sizeof(uint64_t));
    for (size_t i = 0; i < k; i++) {
        uint64_t c = 0; A285219_u128 z;
        for (size_t j = 0; j < k; j++) {
            z = (A285219_u128)a[j] * b[i] + t[j] + c;
            t[j] = (uint64_t)z; c = (uint64_t)(z >> 64);
        }
        z = (A285219_u128)t[k] + c; t[k] = (uint64_t)z; t[k + 1] = (uint64_t)(z >> 64);
        uint64_t m = t[0] * ninv;
        z = (A285219_u128)m * N[0] + t[0]; c = (uint64_t)(z >> 64);
        for (size_t j = 1; j < k; j++) {
            z = (A285219_u128)m * N[j] + t[j] + c;
            t[j - 1] = (uint64_t)z; c = (uint64_t)(z >> 64);
        }
        z = (A285219_u128)t[k] + c; t[k - 1] = (uint64_t)z;
        t[k] = t[k + 1] + (uint64_t)(z >> 64);
    }
    if (t[k] || A285219_cmp(t, N, k) >= 0) A285219_sub(t, N, k);
    memcpy(r, t, k * sizeof(uint64_t));
}

static void A285219_dbl(uint64_t *x, const uint64_t *N, size_t k) {
    uint64_t c = 0;
    for (size_t i = 0; i < k; i++) { uint64_t v = x[i]; x[i] = (v << 1) | c; c = v >> 63; }
    if (c || A285219_cmp(x, N, k) >= 0) A285219_sub(x, N, k);
}

static bool A285219_mr(const uint64_t *N, size_t k, uint64_t ninv, const uint64_t *oneM,
                       const uint64_t *mone, const uint64_t *R2, uint64_t base) {
    uint64_t x[A285219_MAXK], bm[A285219_MAXK], tmp[A285219_MAXK];
    if (base != 2) {
        memset(tmp, 0, k * sizeof(uint64_t)); tmp[0] = base;
        A285219_mmul(bm, tmp, R2, N, k, ninv);
    }
    size_t s = 1;
    while (!((N[s / 64] >> (s % 64)) & 1)) s++;
    size_t top = k * 64 - 1;
    while (!((N[top / 64] >> (top % 64)) & 1)) top--;
    memcpy(x, oneM, k * sizeof(uint64_t));
    for (size_t b = top + 1; b-- > s;) {
        A285219_mmul(x, x, x, N, k, ninv);
        if ((N[b / 64] >> (b % 64)) & 1) {
            if (base == 2) A285219_dbl(x, N, k);
            else A285219_mmul(x, x, bm, N, k, ninv);
        }
    }
    if (A285219_cmp(x, oneM, k) == 0 || A285219_cmp(x, mone, k) == 0) return true;
    for (size_t r = 1; r < s; r++) {
        A285219_mmul(x, x, x, N, k, ninv);
        if (A285219_cmp(x, mone, k) == 0) return true;
        if (A285219_cmp(x, oneM, k) == 0) return false;
    }
    return false;
}

static uint32_t A285219_tp[80];
static int A285219_ntp = 0;
static uint64_t A285219_gprod[80];
static int A285219_gst[81];
static int A285219_ng = 0;

static void A285219_init_trial(void) {
    if (A285219_ntp) return;
    for (uint32_t p = 2; p < 300; p++) {
        bool pr = true;
        for (uint32_t q = 2; q * q <= p; q++) if (p % q == 0) { pr = false; break; }
        if (pr) A285219_tp[A285219_ntp++] = p;
    }
    int i = 0, g = 0;
    while (i < A285219_ntp) {
        uint64_t prod = 1;
        A285219_gst[g] = i;
        while (i < A285219_ntp && prod * A285219_tp[i] < ((uint64_t)1 << 32)) prod *= A285219_tp[i++];
        A285219_gprod[g++] = prod;
    }
    A285219_gst[g] = i;
    A285219_ng = g;
}

static bool A285219_isprime(const uint64_t *x, size_t k) {
    while (k && !x[k - 1]) k--;
    if (k == 0) return false;
    if (k == 1) return is_prime_u64(x[0]);
    for (int g = 0; g < A285219_ng; g++) {
        uint64_t d = A285219_gprod[g], r = 0;
        for (size_t i = k; i-- > 0;) {
            r = ((r << 32) | (x[i] >> 32)) % d;
            r = ((r << 32) | (x[i] & 0xffffffffu)) % d;
        }
        for (int t = A285219_gst[g]; t < A285219_gst[g + 1]; t++)
            if (r % A285219_tp[t] == 0) return false;
    }
    uint64_t inv = x[0];
    for (int it = 0; it < 6; it++) inv *= 2 - x[0] * inv;
    uint64_t ninv = (uint64_t)0 - inv;
    uint64_t oneM[A285219_MAXK], mone[A285219_MAXK], R2[A285219_MAXK];
    memset(oneM, 0, k * sizeof(uint64_t));
    oneM[k - 1] = 1;                          /* 2^(64(k-1)) < N */
    for (int i = 0; i < 64; i++) A285219_dbl(oneM, x, k);   /* R mod N */
    memcpy(mone, x, k * sizeof(uint64_t));
    A285219_sub(mone, oneM, k);
    if (!A285219_mr(x, k, ninv, oneM, mone, R2, 2)) return false;
    memcpy(R2, oneM, k * sizeof(uint64_t));
    for (size_t i = 0; i < 64 * k; i++) A285219_dbl(R2, x, k);  /* R^2 mod N */
    static const uint64_t bases[5] = {3, 5, 7, 11, 13};
    for (int b = 0; b < 5; b++)
        if (!A285219_mr(x, k, ninv, oneM, mone, R2, bases[b])) return false;
    return true;
}

static uint64_t *A285219_found = NULL;
static size_t A285219_nf = 0, A285219_cap = 0;
static idx_t A285219_tested = 0;
static bool A285219_any = false;

seq_t a(idx_t n) {
    A285219_init_trial();
    while (A285219_nf < n) {
        idx_t L = n * n * 62 / 100 + 2000;
        if (A285219_any && L < A285219_tested * 3 / 2) L = A285219_tested * 3 / 2;
        size_t Wd = A285219_wl(L);
        uint64_t *U = (uint64_t *)malloc(((size_t)L + 1) * Wd * sizeof(uint64_t));
        A285219_series(L, Wd, U);
        idx_t start = A285219_any ? A285219_tested + 1 : 0;
        for (idx_t m = start; m <= L; m++) {
            if (A285219_isprime(U + (size_t)m * Wd, Wd)) {
                if (A285219_nf == A285219_cap) {
                    A285219_cap = A285219_cap ? 2 * A285219_cap : 256;
                    A285219_found = (uint64_t *)realloc(A285219_found, A285219_cap * sizeof(uint64_t));
                }
                A285219_found[A285219_nf++] = (uint64_t)m;
            }
        }
        free(U);
        A285219_tested = L;
        A285219_any = true;
    }
    return (seq_t)A285219_found[n - 1];
}
