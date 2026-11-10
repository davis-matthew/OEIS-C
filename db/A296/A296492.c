/*
 * A296492  Decimal expansion of limiting power-ratio for A294170; see
 *          Comments.
 * https://oeis.org/A296492
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = (n-1)-th decimal digit of C = lim_{k->oo}
 *            A294170(k)/phi^k, phi = (1+sqrt(5))/2. With e(k) = b(k) + 2k (so
 *            A294170 has a(k) = a(k-1) + a(k-2) + e(k)), the identity C = lim
 *            (phi*a(N) + a(N-1)) / (sqrt(5)*phi^N) gives C ~= (-1)^N
 *            (P*sqrt(5) + 5Q)/20, where x = a(N)+2a(N-1), y = a(N), P =
 *            x*L...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A296492_B 1000000000u

typedef struct { uint32_t *d; size_t n; } a296492_bn;

static a296492_bn a296492_alloc(size_t n)
{
    a296492_bn r;
    r.d = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
    r.n = n;
    return r;
}
static void a296492_trim(a296492_bn *a) { while (a->n && a->d[a->n - 1] == 0) a->n--; }
static void a296492_free(a296492_bn *a) { free(a->d); a->d = NULL; a->n = 0; }

static a296492_bn a296492_u64(uint64_t v)
{
    a296492_bn r = a296492_alloc(3);
    size_t i = 0;
    while (v) { r.d[i++] = (uint32_t)(v % A296492_B); v /= A296492_B; }
    r.n = i;
    return r;
}

/* a + b + e */
static a296492_bn a296492_add(const a296492_bn *a, const a296492_bn *b, uint64_t e)
{
    size_t n = (a->n > b->n ? a->n : b->n) + 3;
    a296492_bn r = a296492_alloc(n);
    uint64_t c = e;
    for (size_t i = 0; i < n; i++) {
        uint64_t s = c + (i < a->n ? a->d[i] : 0) + (i < b->n ? b->d[i] : 0);
        r.d[i] = (uint32_t)(s % A296492_B);
        c = s / A296492_B;
    }
    a296492_trim(&r);
    return r;
}

static int a296492_cmp(const a296492_bn *a, const a296492_bn *b)
{
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i > 0; i--) {
        if (a->d[i - 1] != b->d[i - 1]) return a->d[i - 1] < b->d[i - 1] ? -1 : 1;
    }
    return 0;
}

/* a - b, a >= b */
static a296492_bn a296492_sub(const a296492_bn *a, const a296492_bn *b)
{
    a296492_bn r = a296492_alloc(a->n);
    int64_t br = 0;
    for (size_t i = 0; i < a->n; i++) {
        int64_t v = (int64_t)a->d[i] - (int64_t)(i < b->n ? b->d[i] : 0) - br;
        if (v < 0) { v += A296492_B; br = 1; } else br = 0;
        r.d[i] = (uint32_t)v;
    }
    a296492_trim(&r);
    return r;
}

static a296492_bn a296492_mul(const a296492_bn *a, const a296492_bn *b)
{
    a296492_bn r = a296492_alloc(a->n + b->n + 1);
    for (size_t i = 0; i < a->n; i++) {
        uint64_t x = a->d[i];
        if (!x) continue;
        uint64_t c = 0;
        size_t j;
        for (j = 0; j < b->n; j++) {
            uint64_t cur = (uint64_t)r.d[i + j] + x * b->d[j] + c;
            r.d[i + j] = (uint32_t)(cur % A296492_B);
            c = cur / A296492_B;
        }
        size_t p = i + j;
        while (c) {
            uint64_t cur = (uint64_t)r.d[p] + c;
            r.d[p] = (uint32_t)(cur % A296492_B);
            c = cur / A296492_B;
            p++;
        }
    }
    a296492_trim(&r);
    return r;
}

static a296492_bn a296492_mulsmall(const a296492_bn *a, uint32_t m)
{
    a296492_bn r = a296492_alloc(a->n + 2);
    uint64_t c = 0;
    for (size_t i = 0; i < a->n + 2; i++) {
        uint64_t v = (uint64_t)(i < a->n ? a->d[i] : 0) * m + c;
        r.d[i] = (uint32_t)(v % A296492_B);
        c = v / A296492_B;
    }
    a296492_trim(&r);
    return r;
}

/* a * 10^M */
static a296492_bn a296492_shift10(const a296492_bn *a, size_t M)
{
    size_t q = M / 9, rr = M % 9;
    uint32_t pw = 1;
    for (size_t i = 0; i < rr; i++) pw *= 10;
    a296492_bn t = a296492_mulsmall(a, pw);
    a296492_bn r = a296492_alloc(t.n + q);
    for (size_t i = 0; i < t.n; i++) r.d[q + i] = t.d[i];
    a296492_free(&t);
    a296492_trim(&r);
    return r;
}

/* signed: (sa*A) + (sb*B), sign in *sr */
static a296492_bn a296492_sadd(int sa, const a296492_bn *A, int sb, const a296492_bn *B, int *sr)
{
    if (sa == sb) { *sr = sa; return a296492_add(A, B, 0); }
    if (a296492_cmp(A, B) >= 0) { *sr = sa; return a296492_sub(A, B); }
    *sr = sb;
    return a296492_sub(B, A);
}

/* fixed-length helpers for the digit-by-digit square root */
static void a296492_fmul(uint32_t *x, size_t L, uint32_t m)
{
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)(v % A296492_B);
        c = v / A296492_B;
    }
}
static void a296492_fadd(uint32_t *x, size_t L, uint32_t v)
{
    uint64_t c = v;
    for (size_t i = 0; i < L && c; i++) {
        uint64_t s = (uint64_t)x[i] + c;
        x[i] = (uint32_t)(s % A296492_B);
        c = s / A296492_B;
    }
}
static int a296492_fcmp(const uint32_t *a, const uint32_t *b, size_t L)
{
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] != b[i - 1]) return a[i - 1] < b[i - 1] ? -1 : 1;
    }
    return 0;
}
static void a296492_fsub(uint32_t *a, const uint32_t *b, size_t L)
{
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += A296492_B; br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
}

/* floor(sqrt(5) * 10^M) */
static a296492_bn a296492_sqrt5(size_t M)
{
    size_t L = (M + 1) / 9 + 5;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (size_t i = 0; i <= M; i++) {
        a296492_fmul(r, L, 100);
        if (i == 0) a296492_fadd(r, L, 5);
        memcpy(d, p, L * sizeof(uint32_t));
        a296492_fmul(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            a296492_fadd(t, L, mid);
            a296492_fmul(t, L, mid);
            if (a296492_fcmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        a296492_fadd(t, L, lo);
        a296492_fmul(t, L, lo);
        a296492_fsub(r, t, L);
        a296492_fmul(p, L, 10);
        a296492_fadd(p, L, lo);
    }
    a296492_bn s = a296492_alloc(L);
    memcpy(s.d, p, L * sizeof(uint32_t));
    a296492_trim(&s);
    free(p); free(r); free(d); free(t);
    return s;
}

static uint8_t *a296492_dg = NULL;
static size_t a296492_valid = 0;

static void a296492_compute(size_t D)
{
    size_t N = 5 * (D + 20) + 60;

    uint64_t sa[256];
    int sn = 2, sbig = 0, pj = 0;
    sa[0] = 1; sa[1] = 2;
    uint64_t lastb = 4;

    a296492_bn A0 = a296492_u64(1), A1 = a296492_u64(2);
    a296492_bn F0 = a296492_u64(0), F1 = a296492_u64(1);
    a296492_bn L0 = a296492_u64(2), L1 = a296492_u64(1);

    for (size_t k = 2; k <= N; k++) {
        uint64_t cand = lastb + 1;
        for (;;) {
            while (pj < sn && sa[pj] < cand) pj++;
            if (pj < sn && sa[pj] == cand) { cand++; continue; }
            break;
        }
        lastb = cand;
        uint64_t ek = cand + 2 * (uint64_t)k;
        if (!sbig) {
            uint64_t v = sa[sn - 1] + sa[sn - 2] + ek;
            if (v > 1000000000000000000ULL || sn >= 255) sbig = 1;
            else sa[sn++] = v;
        }
        a296492_bn A2 = a296492_add(&A1, &A0, ek);
        a296492_free(&A0); A0 = A1; A1 = A2;
        a296492_bn F2 = a296492_add(&F1, &F0, 0);
        a296492_free(&F0); F0 = F1; F1 = F2;
        a296492_bn L2 = a296492_add(&L1, &L0, 0);
        a296492_free(&L0); L0 = L1; L1 = L2;
    }
    /* a(N) = A1, a(N-1) = A0, F(N) = F1, L(N) = L1 */
    a296492_bn x = a296492_add(&A1, &A0, 0);
    {
        a296492_bn x2 = a296492_add(&x, &A0, 0);
        a296492_free(&x); x = x2;
    }
    a296492_bn *y = &A1;

    a296492_bn xL = a296492_mul(&x, &L1);
    a296492_bn yF = a296492_mul(y, &F1);
    a296492_bn yF5 = a296492_mulsmall(&yF, 5);
    int sP;
    a296492_bn P = a296492_sadd(1, &xL, -1, &yF5, &sP);
    a296492_bn yL = a296492_mul(y, &L1);
    a296492_bn xF = a296492_mul(&x, &F1);
    int sQ;
    a296492_bn Q = a296492_sadd(1, &yL, -1, &xF, &sQ);

    size_t M = 9 * P.n + D + 30;
    a296492_bn S = a296492_sqrt5(M);
    a296492_bn T1 = a296492_mul(&P, &S);
    a296492_bn Q5 = a296492_mulsmall(&Q, 5);
    a296492_bn T2 = a296492_shift10(&Q5, M);
    int sR;
    a296492_bn R = a296492_sadd(sP, &T1, sQ, &T2, &sR);
    /* (-1)^N factor; result is positive */
    (void)sR;

    /* R /= 20 */
    {
        uint64_t rem = 0;
        for (size_t i = R.n; i > 0; i--) {
            uint64_t cur = rem * A296492_B + R.d[i - 1];
            R.d[i - 1] = (uint32_t)(cur / 20);
            rem = cur % 20;
        }
        a296492_trim(&R);
    }

    size_t total = 9 * R.n + 9;
    uint8_t *out = (uint8_t *)malloc(total);
    size_t pos = 0;
    if (R.n > 0) {
        uint32_t top = R.d[R.n - 1];
        char tb[12]; int tl = 0;
        while (top) { tb[tl++] = (char)(top % 10); top /= 10; }
        for (int i = tl - 1; i >= 0; i--) out[pos++] = (uint8_t)tb[i];
        for (size_t i = R.n - 1; i > 0; i--) {
            uint32_t v = R.d[i - 1];
            for (int j = 8; j >= 0; j--) { out[pos + j] = (uint8_t)(v % 10); v /= 10; }
            pos += 9;
        }
    }
    free(a296492_dg);
    a296492_dg = out;
    a296492_valid = pos < D ? pos : D;

    a296492_free(&A0); a296492_free(&A1); a296492_free(&F0); a296492_free(&F1);
    a296492_free(&L0); a296492_free(&L1); a296492_free(&x);
    a296492_free(&xL); a296492_free(&yF); a296492_free(&yF5); a296492_free(&P);
    a296492_free(&yL); a296492_free(&xF); a296492_free(&Q);
    a296492_free(&S); a296492_free(&T1); a296492_free(&Q5); a296492_free(&T2);
    a296492_free(&R);
}

seq_t a(idx_t n)
{
    static size_t lastD = 0;
    if (n < 2) return 0;
    size_t idx = (size_t)(n - 2);
    if (a296492_dg == NULL || idx >= a296492_valid) {
        size_t D = lastD * 2;
        if (D < (size_t)n + 20) D = (size_t)n + 20;
        if (D < 120) D = 120;
        lastD = D;
        a296492_compute(D);
    }
    return (seq_t)a296492_dg[idx];
}
