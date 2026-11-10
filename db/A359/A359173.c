/*
 * A359173  Numbers whose square can be expressed as k * A004086(k) with
 *          non-palindromic k.
 * https://oeis.org/A359173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search. Write k = k'*10^t, where k' has no trailing zero, so
 *            that k*R(k) = 10^t * k'*R(k'). Each k' with k'*R(k') = m^2 gives
 *            the terms m*10^s for s >= 0, or for s >= 1 only when k' is a
 *            palindrome. Each k' with k'*R(k') = 10*w^2 gives the terms
 *            10*w*10^s for s >= 0. To find every term <= X it is...
 * Range:     u8 n <= 14, u16 n <= 286, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 70 terms through n = 1941 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

static uint64_t A359173_rev(uint64_t x) {
    uint64_t r = 0;
    while (x > 0) { r = r * 10 + x % 10; x /= 10; }
    return r;
}

static unsigned char A359173_q64[64], A359173_q63[63], A359173_q65[65], A359173_q11[11];
static int A359173_qinit = 0;

static int A359173_issq(uint64_t x, uint64_t *root) {
    if (!A359173_q64[x & 63]) return 0;
    if (!A359173_q63[x % 63]) return 0;
    if (!A359173_q65[x % 65]) return 0;
    if (!A359173_q11[x % 11]) return 0;
    uint64_t s = isqrt_u64(x);
    if (s * s == x) { *root = s; return 1; }
    return 0;
}

static int A359173_cmp(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static uint64_t *A359173_bases = 0;
static size_t A359173_nb = 0, A359173_capb = 0;
static uint64_t *A359173_terms = 0;
static size_t A359173_cnt = 0;
static uint64_t A359173_K = 0, A359173_X = 0;

static void A359173_addbase(uint64_t b) {
    if (A359173_nb == A359173_capb) {
        size_t nc = A359173_capb ? A359173_capb * 2 : 1024;
        A359173_bases = (uint64_t *)realloc(A359173_bases, nc * sizeof(uint64_t));
        A359173_capb = nc;
    }
    A359173_bases[A359173_nb++] = b;
}

seq_t a(idx_t n) {
    if (!A359173_qinit) {
        for (unsigned i = 0; i < 64; i++) A359173_q64[(i * i) % 64] = 1;
        for (unsigned i = 0; i < 63; i++) A359173_q63[(i * i) % 63] = 1;
        for (unsigned i = 0; i < 65; i++) A359173_q65[(i * i) % 65] = 1;
        for (unsigned i = 0; i < 11; i++) A359173_q11[(i * i) % 11] = 1;
        A359173_qinit = 1;
    }
    for (;;) {
        if (A359173_cnt >= (size_t)n) return (seq_t)A359173_terms[n - 1];
        uint64_t X = A359173_X ? A359173_X * 4 : 1000;
        uint64_t Kn = X / 1000 * 3163 + (X % 1000) * 3163 / 1000 + 2;
        for (uint64_t k = A359173_K + 1; k <= Kn; k++) {
            if (k % 10 == 0) continue;
            uint64_t r = A359173_rev(k);
            uint64_t P = k * r, s;
            if (A359173_issq(P, &s)) {
                if (r != k) A359173_addbase(s);
                else A359173_addbase(10 * s);
            } else if (P % 10 == 0 && A359173_issq(P / 10, &s)) {
                A359173_addbase(10 * s);
            }
        }
        A359173_K = Kn;
        A359173_X = X;
        /* rebuild list of all terms <= X */
        size_t cap = 1024, c = 0;
        uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
        for (size_t i = 0; i < A359173_nb; i++) {
            uint64_t v = A359173_bases[i];
            while (v <= X) {
                if (c == cap) { cap *= 2; t = (uint64_t *)realloc(t, cap * sizeof(uint64_t)); }
                t[c++] = v;
                v *= 10;
            }
        }
        qsort(t, c, sizeof(uint64_t), A359173_cmp);
        size_t u = 0;
        for (size_t i = 0; i < c; i++)
            if (u == 0 || t[u - 1] != t[i]) t[u++] = t[i];
        if (A359173_terms) free(A359173_terms);
        A359173_terms = t;
        A359173_cnt = u;
    }
}
