/*
 * A384659  Numbers k such that A384655(k)/k > A384655(m)/m for all m < k.
 * https://oeis.org/A384659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) is the n-th k with A384655(k)/k > A384655(m)/m for
 *            all m < k, where A384655(k)/k = Sum_{j>=1} (1 - Product_{p^j |
 *            k} (1 - p^(-j))). Only k whose exponents do not increase over
 *            consecutive primes 2,3,5,... can set a record. The code searches
 *            these k band by band in bit length, using b...
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 65, u64 n <= 135,
 *            u128 n <= 283, big any n
 * Validated: standard, pass, 57 terms through n = 348 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define Q659_BL   24
#define Q659_NP   64
#define Q659_KMAX 340
#define Q659_MAXB 320
#define Q659_STEP 8
#define Q659_EPS  1e-9

typedef struct { uint32_t d[Q659_BL]; int len; } q659_bn;
typedef struct { q659_bn n; double r; uint16_t e[Q659_NP]; } q659_cand;

static uint32_t q659_pr[Q659_NP];
static int q659_TM = 0;
static double q659_LP[Q659_NP];
static q659_bn q659_P[Q659_NP];
static double q659_G[Q659_KMAX + 1][Q659_NP];
static double q659_S[Q659_KMAX + 1][Q659_NP];
static bool q659_inited = false;

static q659_bn *q659_recs = NULL;
static size_t q659_nrec = 0, q659_caprec = 0;
static q659_cand q659_last;
static int q659_Bdone = 0;

static int q659_Bold, q659_Bnew;
static double q659_Rprev;
static uint16_t q659_cur[Q659_NP];
static q659_cand *q659_surv = NULL;
static size_t q659_ns = 0, q659_caps = 0;

static void q659_one(q659_bn *a) { a->d[0] = 1; a->len = 1; }
static void q659_trim(q659_bn *a) { while (a->len > 0 && a->d[a->len - 1] == 0) a->len--; }

static void q659_mul_small(q659_bn *a, uint32_t m)
{
    uint64_t c = 0;
    for (int i = 0; i < a->len; ++i) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t; c = t >> 32;
    }
    if (c && a->len < Q659_BL) a->d[a->len++] = (uint32_t)c;
}

static void q659_mul(const q659_bn *a, const q659_bn *b, q659_bn *r)
{
    uint32_t res[2 * Q659_BL];
    int L = a->len + b->len;
    for (int i = 0; i < L; ++i) res[i] = 0;
    for (int i = 0; i < a->len; ++i) {
        uint64_t c = 0;
        for (int j = 0; j < b->len; ++j) {
            uint64_t t = (uint64_t)a->d[i] * b->d[j] + res[i + j] + c;
            res[i + j] = (uint32_t)t; c = t >> 32;
        }
        res[i + b->len] = (uint32_t)c;
    }
    if (L > Q659_BL) L = Q659_BL;
    for (int i = 0; i < L; ++i) r->d[i] = res[i];
    r->len = L;
    q659_trim(r);
}

static int q659_bitlen(const q659_bn *a)
{
    if (a->len == 0) return 0;
    uint32_t t = a->d[a->len - 1];
    int b = 0;
    while (t) { b++; t >>= 1; }
    return 32 * (a->len - 1) + b;
}

static int q659_cmp(const q659_bn *a, const q659_bn *b)
{
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (int i = a->len - 1; i >= 0; --i)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

static void q659_add(q659_bn *a, const q659_bn *b)
{
    int L = a->len > b->len ? a->len : b->len;
    uint64_t c = 0;
    for (int i = 0; i < L; ++i) {
        uint64_t t = c + (i < a->len ? a->d[i] : 0) + (i < b->len ? b->d[i] : 0);
        a->d[i] = (uint32_t)t; c = t >> 32;
    }
    a->len = L;
    if (c && a->len < Q659_BL) a->d[a->len++] = (uint32_t)c;
}

static void q659_sub(q659_bn *a, const q659_bn *b) /* a >= b */
{
    int64_t br = 0;
    for (int i = 0; i < a->len; ++i) {
        int64_t t = (int64_t)a->d[i] - (i < b->len ? (int64_t)b->d[i] : 0) - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a->d[i] = (uint32_t)t;
    }
    q659_trim(a);
}

static void q659_init(void)
{
    int c = 0;
    for (uint32_t x = 2; c < Q659_NP; ++x) {
        bool pr = true;
        for (int i = 0; i < c && q659_pr[i] * q659_pr[i] <= x; ++i)
            if (x % q659_pr[i] == 0) { pr = false; break; }
        if (pr) q659_pr[c++] = x;
    }
    q659_one(&q659_P[0]);
    q659_LP[0] = 0.0;
    q659_TM = 0;
    for (int t = 1; t < Q659_NP; ++t) {
        q659_P[t] = q659_P[t - 1];
        q659_mul_small(&q659_P[t], q659_pr[t - 1]);
        if (q659_bitlen(&q659_P[t]) > Q659_MAXB + 10) break;
        q659_LP[t] = q659_LP[t - 1] + log2((double)q659_pr[t - 1]);
        q659_TM = t;
    }
    double pw[Q659_NP];
    for (int t = 0; t < Q659_NP; ++t) pw[t] = 1.0;
    for (int t = 0; t < Q659_NP; ++t) { q659_G[0][t] = 0.0; q659_S[0][t] = 0.0; }
    for (int k = 1; k <= Q659_KMAX; ++k) {
        q659_G[k][0] = 0.0;
        for (int t = 1; t <= q659_TM; ++t) {
            pw[t] /= (double)q659_pr[t - 1];
            double g = q659_G[k][t - 1];
            q659_G[k][t] = g + (1.0 - g) * pw[t];
        }
        for (int t = 0; t <= q659_TM; ++t)
            q659_S[k][t] = q659_S[k - 1][t] + q659_G[k][t];
    }
    q659_inited = true;
}

/* exact f = A384655(n) for exponent vector e */
static void q659_exact(const uint16_t *e, q659_bn *n, q659_bn *f)
{
    q659_one(n);
    for (int i = 0; i < Q659_NP && e[i] > 0; ++i)
        for (int c = 0; c < e[i]; ++c) q659_mul_small(n, q659_pr[i]);
    f->len = 0;
    q659_bn one; q659_one(&one);
    for (int k = 1; k <= (int)e[0]; ++k) {
        q659_bn M; q659_one(&M);
        for (int i = 0; i < Q659_NP && e[i] > 0; ++i) {
            uint32_t p = q659_pr[i];
            if (e[i] >= k) {
                for (int c = 0; c < e[i] - k; ++c) q659_mul_small(&M, p);
                q659_bn pk, tmp; q659_one(&pk);
                for (int c = 0; c < k; ++c) q659_mul_small(&pk, p);
                q659_sub(&pk, &one);
                q659_mul(&M, &pk, &tmp);
                M = tmp;
            } else {
                for (int c = 0; c < e[i]; ++c) q659_mul_small(&M, p);
            }
        }
        q659_bn D = *n;
        q659_sub(&D, &M);
        q659_add(f, &D);
    }
}

static int q659_cmp_ratio(const q659_cand *A, const q659_cand *B)
{
    if (A->r > B->r + Q659_EPS) return 1;
    if (A->r < B->r - Q659_EPS) return -1;
    q659_bn nA, fA, nB, fB, X, Y;
    q659_exact(A->e, &nA, &fA);
    q659_exact(B->e, &nB, &fB);
    q659_mul(&fA, &nB, &X);
    q659_mul(&fB, &nA, &Y);
    return q659_cmp(&X, &Y);
}

static double q659_ub(int Kc, int t, double budget)
{
    budget += 1e-6;
    if (budget < 1.0) return 0.0;
    int m = (int)budget;
    int T = t;
    while (T > 0 && q659_LP[T] > budget) T--;
    if (T == 0) return 0.0;
    int hi = Kc + m;
    if (hi > Q659_KMAX) hi = Q659_KMAX;
    if (hi <= Kc) return 0.0;
    return q659_S[hi][T] - q659_S[Kc][T];
}

static void q659_dfs(int K, int tK, const q659_bn *n, double lg, double r)
{
    int bl = q659_bitlen(n);
    if (bl > q659_Bold && r > q659_Rprev - Q659_EPS) {
        if (q659_ns == q659_caps) {
            q659_caps = q659_caps ? q659_caps * 2 : 1024;
            q659_surv = (q659_cand *)realloc(q659_surv, q659_caps * sizeof(q659_cand));
        }
        q659_cand *c = &q659_surv[q659_ns++];
        c->n = *n; c->r = r;
        for (int i = 0; i < Q659_NP; ++i) c->e[i] = q659_cur[i];
    }
    if (K + 1 > Q659_KMAX) return;
    int tmax = tK < q659_TM ? tK : q659_TM;
    for (int t = 1; t <= tmax; ++t) {
        double lg2 = lg + q659_LP[t];
        if (lg2 > (double)q659_Bnew + 1e-6) break;
        q659_bn n2;
        q659_mul(n, &q659_P[t], &n2);
        if (q659_bitlen(&n2) > q659_Bnew) break;
        double r2 = r + q659_G[K + 1][t];
        double ub = q659_ub(K + 1, t, (double)q659_Bnew - lg2);
        if (r2 + ub < q659_Rprev - Q659_EPS) continue;
        for (int i = 0; i < t; ++i) q659_cur[i]++;
        q659_dfs(K + 1, t, &n2, lg2, r2);
        for (int i = 0; i < t; ++i) q659_cur[i]--;
    }
}

static int q659_sortcmp(const void *x, const void *y)
{
    return q659_cmp(&((const q659_cand *)x)->n, &((const q659_cand *)y)->n);
}

static void q659_extend(void)
{
    q659_Bold = q659_Bdone;
    q659_Bnew = q659_Bdone + Q659_STEP;
    if (q659_Bnew > Q659_MAXB) q659_Bnew = Q659_MAXB;
    q659_Rprev = q659_nrec ? q659_last.r : -1.0;
    q659_ns = 0;
    for (int i = 0; i < Q659_NP; ++i) q659_cur[i] = 0;
    q659_bn root; q659_one(&root);
    q659_dfs(0, q659_TM, &root, 0.0, 0.0);
    if (q659_ns > 1) qsort(q659_surv, q659_ns, sizeof(q659_cand), q659_sortcmp);
    for (size_t i = 0; i < q659_ns; ++i) {
        q659_cand *s = &q659_surv[i];
        if (q659_nrec == 0 || q659_cmp_ratio(s, &q659_last) > 0) {
            if (q659_nrec == q659_caprec) {
                q659_caprec = q659_caprec ? q659_caprec * 2 : 256;
                q659_recs = (q659_bn *)realloc(q659_recs, q659_caprec * sizeof(q659_bn));
            }
            q659_recs[q659_nrec++] = s->n;
            q659_last = *s;
        }
    }
    q659_Bdone = q659_Bnew;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!q659_inited) q659_init();
    while (q659_nrec < n && q659_Bdone < Q659_MAXB) q659_extend();
    if (n > q659_nrec) return 0;
    const q659_bn *v = &q659_recs[n - 1];
    seq_t r = 0;
    for (int i = v->len - 1; i >= 0; --i) {
        r = r * 65536;
        r = r * 65536;
        r += (seq_t)v->d[i];
    }
    return r;
}
