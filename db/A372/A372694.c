/*
 * A372694  Numbers k such that A372692(k) = A372692(k+1) = A372692(k+2) > 1.
 * https://oeis.org/A372694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. A372692(m) depends only on the "bad part" b(m) of m: the
 *            prime powers p^e in m whose exponent e is not of the form 2^j-1.
 *            Up to a bound X, list every such bad part r <= X, grouped by
 *            A372692(r). A term k needs pairwise coprime r0 | k, r1 | k+1, r2
 *            | k+2 from one group, so k is found by CR...
 * Range:     u8 none, u16 none, u32 n <= 238, u64 n <= 1200, u128 n <= 1200,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 1076 (2026-11-10)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

typedef struct { uint64_t g, r; } A372694_pair;

static uint64_t *A372694_P = 0;
static size_t A372694_nP = 0;
static A372694_pair *A372694_bad = 0;
static size_t A372694_nbad = 0, A372694_capbad = 0;
static uint64_t *A372694_terms = 0;
static size_t A372694_cnt = 0, A372694_cap = 0;
static uint64_t A372694_X = 0;

static uint64_t A372694_gpe(uint64_t p, unsigned e) {
    uint64_t prod = 1;
    for (unsigned k = 1; k < e; k <<= 1) {
        if ((e & k) == 0) {
            uint64_t pw = 1;
            for (unsigned i = 0; i < k; i++) pw *= p;
            prod *= pw + 1;
        }
    }
    return prod;
}

static void A372694_sieve(uint64_t lim) {
    unsigned char *s = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A372694_P);
    A372694_P = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim / 2 + 16));
    A372694_nP = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!s[i]) {
            A372694_P[A372694_nP++] = i;
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 1;
        }
    }
    free(s);
}

static void A372694_push_bad(uint64_t r, uint64_t g) {
    if (A372694_nbad == A372694_capbad) {
        A372694_capbad = A372694_capbad ? A372694_capbad * 2 : 4096;
        A372694_bad = (A372694_pair *)realloc(A372694_bad, A372694_capbad * sizeof(A372694_pair));
    }
    A372694_bad[A372694_nbad].g = g;
    A372694_bad[A372694_nbad].r = r;
    A372694_nbad++;
}

static void A372694_dfs(size_t idx, uint64_t cur, uint64_t gc, uint64_t X) {
    uint64_t lim = X / cur;
    for (size_t i = idx; i < A372694_nP; i++) {
        uint64_t p = A372694_P[i];
        if (p > lim / p) break;
        uint64_t pe = p * p;
        unsigned e = 2;
        for (;;) {
            if (((e + 1) & e) != 0) {
                uint64_t g = gc * A372694_gpe(p, e);
                A372694_push_bad(cur * pe, g);
                A372694_dfs(i + 1, cur * pe, g, X);
            }
            if (pe > lim / p) break;
            pe *= p;
            e++;
        }
    }
}

static int A372694_cmp(const void *x, const void *y) {
    const A372694_pair *a = (const A372694_pair *)x, *b = (const A372694_pair *)y;
    if (a->g != b->g) return a->g < b->g ? -1 : 1;
    if (a->r != b->r) return a->r < b->r ? -1 : 1;
    return 0;
}

static int A372694_cmpu(const void *x, const void *y) {
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static uint64_t A372694_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A372694_inv(uint64_t a, uint64_t m) {
    int64_t t = 0, nt = 1, r = (int64_t)m, nr = (int64_t)(a % m);
    while (nr) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)m;
    return (uint64_t)t;
}

static bool A372694_check(uint64_t m, uint64_t V) {
    uint64_t gv = 1;
    for (size_t i = 0; i < A372694_nP; i++) {
        uint64_t p = A372694_P[i];
        if (p * p * p > m) break;
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            gv *= A372694_gpe(p, e);
            if (gv > V || V % gv != 0) return false;
        }
    }
    if (m > 1) {
        uint64_t s = isqrt_u64(m);
        while (s * s > m) s--;
        while ((s + 1) * (s + 1) <= m) s++;
        if (s * s == m) gv *= s + 1;
    }
    return gv == V;
}

static void A372694_verify(uint64_t k, uint64_t V) {
    if (A372694_check(k, V) && A372694_check(k + 1, V) && A372694_check(k + 2, V)) {
        if (A372694_cnt == A372694_cap) {
            A372694_cap = A372694_cap ? A372694_cap * 2 : 256;
            A372694_terms = (uint64_t *)realloc(A372694_terms, A372694_cap * sizeof(uint64_t));
        }
        A372694_terms[A372694_cnt++] = k;
    }
}

static void A372694_compute(uint64_t X) {
    uint64_t sq = isqrt_u64(X) + 2;
    A372694_sieve(sq);
    A372694_nbad = 0;
    A372694_cnt = 0;
    A372694_dfs(0, 1, 1, X);
    qsort(A372694_bad, A372694_nbad, sizeof(A372694_pair), A372694_cmp);

    size_t i = 0;
    while (i < A372694_nbad) {
        size_t j = i;
        uint64_t V = A372694_bad[i].g;
        while (j < A372694_nbad && A372694_bad[j].g == V) j++;
        if (j - i >= 3) {
            for (size_t a = i; a < j; a++) {
                uint64_t r0 = A372694_bad[a].r;
                for (size_t b = i; b < j; b++) {
                    if (b == a) continue;
                    uint64_t r1 = A372694_bad[b].r;
                    if (A372694_gcd(r0, r1) != 1) continue;
                    uint64_t iv = A372694_inv(r0 % r1, r1);
                    uint64_t t0 = (r1 - iv) % r1;
                    unsigned __int128 k0w = (unsigned __int128)r0 * t0;
                    if (k0w > X) continue;
                    uint64_t k0 = (uint64_t)k0w;
                    unsigned __int128 Mw = (unsigned __int128)r0 * r1;
                    if (Mw > X) {
                        for (size_t c = i; c < j; c++) {
                            if (c == a || c == b) continue;
                            if ((k0 + 2) % A372694_bad[c].r == 0) A372694_verify(k0, V);
                        }
                    } else {
                        uint64_t M = (uint64_t)Mw;
                        for (size_t c = i; c < j; c++) {
                            if (c == a || c == b) continue;
                            uint64_t r2 = A372694_bad[c].r;
                            if (A372694_gcd(M, r2) != 1) continue;
                            uint64_t a2 = (r2 - (k0 + 2) % r2) % r2;
                            uint64_t im = A372694_inv(M % r2, r2);
                            uint64_t u0 = (uint64_t)(((unsigned __int128)a2 * im) % r2);
                            unsigned __int128 k = (unsigned __int128)k0 + (unsigned __int128)M * u0;
                            unsigned __int128 R = (unsigned __int128)M * r2;
                            for (; k <= X; k += R) A372694_verify((uint64_t)k, V);
                        }
                    }
                }
            }
        }
        i = j;
    }

    qsort(A372694_terms, A372694_cnt, sizeof(uint64_t), A372694_cmpu);
    size_t w = 0;
    for (size_t t = 0; t < A372694_cnt; t++) {
        if (w == 0 || A372694_terms[w - 1] != A372694_terms[t]) A372694_terms[w++] = A372694_terms[t];
    }
    A372694_cnt = w;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    while (A372694_cnt < (size_t)n) {
        A372694_X = A372694_X ? A372694_X * 4 : ((uint64_t)1 << 27);
        A372694_compute(A372694_X);
    }
    return (seq_t)A372694_terms[n - 1];
}
