/*
 * A362404  Numbers k such that k and k+1 are both in A362401.
 * https://oeis.org/A362404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For m = q*r, with q squarefree, r powerful and gcd(q,r)
 *            = 1, A162296(m) = sigma(q)*A162296(r). An odd value of A162296
 *            comes only from m = s^2 or m = 2*s^2. So list every odd value v
 *            <= X, then test whether v-1 or v+1 equals sigma(q)*A162296(r)
 *            for a powerful r and a squarefree q coprime...
 * Range:     u8 n <= 7, u16 n <= 138, u32 n <= 3557, u64 n <= 3557,
 *            u128 n <= 3557, big any n
 * Validated: standard, pass, 64 terms through n = 3509 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

typedef struct { uint64_t g, r; } A362404_gr;

static uint64_t *A362404_P = 0;
static size_t A362404_NP = 0;
static A362404_gr *A362404_GR = 0;
static size_t A362404_ngr = 0, A362404_capgr = 0;
static uint64_t A362404_Wmax = 0;

static uint64_t *A362404_T = 0;
static size_t A362404_cnt = 0;
static uint64_t A362404_X = 0;

static uint64_t A362404_D[20000];
static size_t A362404_nd = 0;
static uint64_t A362404_good[20000];
static size_t A362404_ngood = 0;

static int A362404_cmpu(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x > y) - (x < y);
}

static int A362404_cmpgr(const void *a, const void *b)
{
    const A362404_gr *x = (const A362404_gr *)a, *y = (const A362404_gr *)b;
    if (x->g != y->g) return (x->g > y->g) - (x->g < y->g);
    return (x->r > y->r) - (x->r < y->r);
}

static void A362404_primes(uint64_t lim)
{
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j;
    free(A362404_P);
    A362404_P = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)lim + 1));
    A362404_NP = 0;
    for (i = 2; i <= lim; i++) {
        if (!c[i]) {
            A362404_P[A362404_NP++] = i;
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
}

static void A362404_pushgr(uint64_t g, uint64_t r)
{
    if (A362404_ngr == A362404_capgr) {
        A362404_capgr = A362404_capgr ? A362404_capgr * 2 : 4096;
        A362404_GR = (A362404_gr *)realloc(A362404_GR, A362404_capgr * sizeof(A362404_gr));
    }
    A362404_GR[A362404_ngr].g = g;
    A362404_GR[A362404_ngr].r = r;
    A362404_ngr++;
}

static void A362404_powrec(size_t i, uint64_t r, uint64_t sig, uint64_t sf)
{
    size_t j;
    uint64_t W = A362404_Wmax;
    if (r > 1) {
        uint64_t g = sig - sf;
        if (g > W) return;          /* descendants only grow */
        A362404_pushgr(g, r);
    }
    for (j = i; j < A362404_NP; j++) {
        uint64_t p = A362404_P[j];
        uint64_t pe, s;
        if (p * p > W / r) break;
        pe = p * p;
        s = 1 + p + p * p;
        for (;;) {
            A362404_powrec(j + 1, r * pe, sig * s, sf * (1 + p));
            if (pe > W / r / p) break;
            pe *= p;
            s += pe;
        }
    }
}

static bool A362404_dfs(uint64_t t, size_t idx, uint64_t r)
{
    size_t i;
    if (t == 1) return true;
    for (i = idx; i < A362404_ngood; i++) {
        uint64_t e = A362404_good[i];
        if (e > t) break;
        if (t % e == 0 && r % (e - 1) != 0) {
            if (A362404_dfs(t / e, i + 1, r)) return true;
        }
    }
    return false;
}

static bool A362404_inrange(uint64_t w)
{
    uint64_t pr[64];
    int ex[64];
    int nf = 0, k;
    uint64_t x = w;
    size_t i;
    if (w == 0) return false;
    for (i = 0; i < A362404_NP; i++) {
        uint64_t p = A362404_P[i];
        if (p * p > x) break;
        if (x % p == 0) {
            int e = 0;
            while (x % p == 0) { x /= p; e++; }
            pr[nf] = p; ex[nf] = e; nf++;
        }
    }
    if (x > 1) { pr[nf] = x; ex[nf] = 1; nf++; }

    A362404_nd = 1;
    A362404_D[0] = 1;
    for (k = 0; k < nf; k++) {
        size_t base = A362404_nd, b;
        uint64_t pe = 1;
        int e;
        for (e = 1; e <= ex[k]; e++) {
            pe *= pr[k];
            for (b = 0; b < base; b++) A362404_D[A362404_nd++] = A362404_D[b] * pe;
        }
    }
    qsort(A362404_D, A362404_nd, sizeof(uint64_t), A362404_cmpu);
    A362404_ngood = 0;
    for (i = 0; i < A362404_nd; i++) {
        uint64_t e = A362404_D[i];
        if (e >= 3 && is_prime_u64(e - 1)) A362404_good[A362404_ngood++] = e;
    }
    for (i = 0; i < A362404_nd; i++) {
        uint64_t d = A362404_D[i];
        size_t lo = 0, hi = A362404_ngr;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (A362404_GR[mid].g < d) lo = mid + 1; else hi = mid;
        }
        while (lo < A362404_ngr && A362404_GR[lo].g == d) {
            if (A362404_dfs(w / d, 0, A362404_GR[lo].r)) return true;
            lo++;
        }
    }
    return false;
}

/* A162296(m) for m = s^2 (two = 0) or m = 2*s^2 (two = 1) */
static uint64_t A362404_val(uint64_t s, int two)
{
    uint64_t sig = 1, sf = 1, x = s;
    size_t i;
    int saw2 = 0;
    for (i = 0; i < A362404_NP; i++) {
        uint64_t p = A362404_P[i];
        if (p * p > x) break;
        if (x % p == 0) {
            int e = 0, E, t;
            uint64_t term = 1, pe = 1;
            while (x % p == 0) { x /= p; e++; }
            E = 2 * e + ((two && p == 2) ? 1 : 0);
            if (p == 2) saw2 = 1;
            for (t = 0; t < E; t++) { pe *= p; term += pe; }
            sig *= term;
            sf *= (1 + p);
        }
    }
    if (x > 1) {
        int E = 2 + ((two && x == 2) ? 1 : 0);
        if (x == 2) saw2 = 1;
        sig *= (E == 2) ? (1 + x + x * x) : (1 + x + x * x + x * x * x);
        sf *= (1 + x);
    }
    if (two && !saw2) { sig *= 3; sf *= 3; }
    return sig - sf;
}

static void A362404_build(uint64_t X)
{
    uint64_t s, lim;
    uint64_t *odd = 0;
    size_t nodd = 0, cap = 0, i;

    A362404_Wmax = X + 1;
    lim = 2;
    while ((lim + 1) * (lim + 1) <= A362404_Wmax) lim++;
    A362404_primes(lim + 2);

    A362404_ngr = 0;
    A362404_powrec(0, 1, 1, 1);
    qsort(A362404_GR, A362404_ngr, sizeof(A362404_gr), A362404_cmpgr);

    for (s = 2; s * s <= X; s++) {
        int two;
        for (two = 0; two <= 1; two++) {
            uint64_t m = s * s * (two ? 2 : 1), v;
            if (m > X) continue;
            v = A362404_val(s, two);
            if ((v & 1) && v <= X) {
                if (nodd == cap) {
                    cap = cap ? cap * 2 : 4096;
                    odd = (uint64_t *)realloc(odd, cap * sizeof(uint64_t));
                }
                odd[nodd++] = v;
            }
        }
    }
    if (nodd) qsort(odd, nodd, sizeof(uint64_t), A362404_cmpu);

    free(A362404_T);
    A362404_T = (uint64_t *)malloc(sizeof(uint64_t) * (2 * nodd + 1));
    A362404_cnt = 0;
    for (i = 0; i < nodd; i++) {
        uint64_t v = odd[i];
        if (i > 0 && odd[i - 1] == v) continue;
        if (v >= 2 && A362404_inrange(v - 1)) A362404_T[A362404_cnt++] = v - 1;
        if (v + 1 <= X && A362404_inrange(v + 1)) A362404_T[A362404_cnt++] = v;
    }
    free(odd);
    qsort(A362404_T, A362404_cnt, sizeof(uint64_t), A362404_cmpu);
    A362404_X = X;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    if (A362404_X == 0) A362404_build((uint64_t)1 << 16);
    while ((uint64_t)A362404_cnt < nn) A362404_build(A362404_X * 4);
    return (seq_t)A362404_T[nn - 1];
}
