/*
 * A395977  Denominators of rational coefficients related to asymptotic of
 *          A336293.
 * https://oeis.org/A395977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(c(n)), where A336293(n) ~
 *            n^n*exp(4*sqrt(n)-n-2)/sqrt(8*Pi) * Sum_{k>=0} c(k)/n^(k/2),
 *            c(0) = 1. The c(k) are found by putting this ansatz into the
 *            recurrence n*b(n) = (3n^2+n-1)*b(n-1) - (n-1)^2*(3n+1)*b(n-2) +
 *            (n-2)^2*(n-1)^2*b(n-3) and expanding in x = n^(-1/2) with exact
 *            ra...
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 91 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

typedef struct { seq_t n, d; int neg; } fr_t;

static seq_t fr_g(seq_t a, seq_t b)
{
    while (b != 0) { seq_t t = a % b; a = b; b = t; }
    return a;
}

static fr_t fr_zero(void)
{
    fr_t r; r.n = 0; r.d = 1; r.neg = 0; return r;
}

static fr_t fr_norm(seq_t n, seq_t d, int neg)
{
    fr_t r;
    if (n == 0) return fr_zero();
    seq_t g = fr_g(n, d);
    if (g == 0) g = 1;
    r.n = n / g; r.d = d / g; r.neg = neg;
    return r;
}

static fr_t fr_make(long long p, long long q)
{
    int neg = 0;
    if (q < 0) { q = -q; p = -p; }
    if (p < 0) { neg = 1; p = -p; }
    return fr_norm((seq_t)p, (seq_t)q, neg);
}

static fr_t fr_neg(fr_t a)
{
    if (a.n != 0) a.neg = !a.neg;
    return a;
}

static fr_t fr_add(fr_t a, fr_t b)
{
    if (a.n == 0) return b;
    if (b.n == 0) return a;
    seq_t g = fr_g(a.d, b.d);
    if (g == 0) g = 1;
    seq_t ad = a.d / g, bd = b.d / g;
    seq_t x = a.n * bd, y = b.n * ad;
    seq_t den = ad * b.d;
    seq_t num; int neg;
    if (a.neg == b.neg) { num = x + y; neg = a.neg; }
    else if (x >= y) { num = x - y; neg = a.neg; }
    else { num = y - x; neg = b.neg; }
    if (num == 0) return fr_zero();
    return fr_norm(num, den, neg);
}

static fr_t fr_mul(fr_t a, fr_t b)
{
    if (a.n == 0 || b.n == 0) return fr_zero();
    seq_t g1 = fr_g(a.n, b.d);
    seq_t g2 = fr_g(b.n, a.d);
    if (g1 == 0) g1 = 1;
    if (g2 == 0) g2 = 1;
    fr_t r;
    r.n = (a.n / g1) * (b.n / g2);
    r.d = (a.d / g2) * (b.d / g1);
    r.neg = (a.neg != b.neg);
    return r;
}

static fr_t fr_mul_int(fr_t a, long long c)
{
    return fr_mul(a, fr_make(c, 1));
}

typedef struct { seq_t *n; seq_t *d; char *s; size_t len; } frarr_t;

static frarr_t fa_new(size_t len)
{
    frarr_t a;
    size_t L = len ? len : 1;
    a.n = mem_new(seq_t, L);
    a.d = mem_new(seq_t, L);
    a.s = (char *)calloc(L, 1);
    a.len = len;
    for (size_t i = 0; i < L; i++) { a.n[i] = 0; a.d[i] = 1; a.s[i] = 0; }
    return a;
}

static void fa_free(frarr_t *a)
{
    if (a->n) mem_free(a->n);
    if (a->d) mem_free(a->d);
    if (a->s) free(a->s);
    a->n = NULL; a->d = NULL; a->s = NULL; a->len = 0;
}

static fr_t fa_get(const frarr_t *a, size_t i)
{
    fr_t r;
    r.n = a->n[i]; r.d = a->d[i]; r.neg = a->s[i];
    return r;
}

static void fa_set(frarr_t *a, size_t i, fr_t v)
{
    a->n[i] = v.n; a->d[i] = v.d; a->s[i] = (char)v.neg;
}

/* Q_j(y) = P_j(n) / n^(j+1), y = 1/n */
static const int Q_tab[3][5] = {
    { 3,  1,  -1,   0, 0 },
    { -3, 5,  -1,  -1, 0 },
    { 1, -6,  13, -12, 4 }
};

static frarr_t Mt = { NULL, NULL, NULL, 0 };
static size_t M_D = 0, M_U = 0;
static frarr_t Cc = { NULL, NULL, NULL, 0 };
static size_t C_cnt = 0;

static void build_M(size_t D)
{
    size_t U = D + 1, I = D / 2 + 1;
    fa_free(&Mt);
    Mt = fa_new(I * U);
    M_D = D; M_U = U;

    frarr_t G[3];
    for (int j = 1; j <= 3; j++) {
        /* L_j(x) coefficients */
        frarr_t Lc = fa_new(U);
        fr_t w = fr_make(1, 1);      /* w_i = (-1)^i binomial(1/2, i) */
        fr_t pj = fr_make(1, 1);     /* j^i */
        for (size_t i = 1; 2 * i - 1 <= D; i++) {
            w = fr_mul(w, fr_make(2 * (long long)i - 3, 2 * (long long)i));
            pj = fr_mul_int(pj, j);
            fa_set(&Lc, 2 * i - 1, fr_mul_int(fr_mul(w, pj), 4));
        }
        pj = fr_make(j, 1);          /* j^(t+1) */
        for (size_t t = 1; 2 * t <= D; t++) {
            pj = fr_mul_int(pj, j);
            fa_set(&Lc, 2 * t,
                   fr_mul(pj, fr_make(1, (long long)t * (long long)(t + 1))));
        }
        /* E = exp(L): s*e_s = Sum_{m=1..s} m*L_m*e_{s-m} */
        frarr_t E = fa_new(U);
        fa_set(&E, 0, fr_make(1, 1));
        for (size_t s = 1; s <= D; s++) {
            fr_t sum = fr_zero();
            for (size_t m = 1; m <= s; m++) {
                fr_t lm = fa_get(&Lc, m);
                if (lm.n == 0) continue;
                sum = fr_add(sum, fr_mul(fr_mul_int(lm, (long long)m), fa_get(&E, s - m)));
            }
            fa_set(&E, s, fr_mul(sum, fr_make(1, (long long)s)));
        }
        /* G_j = Q_j(x^2) * E */
        G[j - 1] = fa_new(U);
        for (size_t s = 0; s <= D; s++) {
            fr_t acc = fr_zero();
            for (size_t t = 0; t < 5 && 2 * t <= s; t++) {
                int q = Q_tab[j - 1][t];
                if (q != 0)
                    acc = fr_add(acc, fr_mul_int(fa_get(&E, s - 2 * t), q));
            }
            fa_set(&G[j - 1], s, acc);
        }
        fa_free(&Lc);
        fa_free(&E);
    }

    /* M_i = Sum_j j^i * G_j */
    for (size_t i = 0; i < I; i++) {
        for (size_t u = 0; u < U; u++) {
            fr_t sum = fr_zero();
            for (int j = 0; j < 3; j++)
                sum = fr_add(sum, fa_get(&G[j], u));
            fa_set(&Mt, i * U + u, sum);
        }
        if (i + 1 < I)
            for (int j = 0; j < 3; j++)
                for (size_t u = 0; u < U; u++)
                    fa_set(&G[j], u, fr_mul_int(fa_get(&G[j], u), j + 1));
    }
    for (int j = 0; j < 3; j++) fa_free(&G[j]);
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t need = N + 4;
    if (M_D < need)
        build_M(2 * need);

    if (Cc.len <= N) {
        size_t cap = 2 * N + 8;
        frarr_t nc = fa_new(cap);
        for (size_t i = 0; i < C_cnt; i++) fa_set(&nc, i, fa_get(&Cc, i));
        fa_free(&Cc);
        Cc = nc;
    }
    if (C_cnt == 0) {
        fa_set(&Cc, 0, fr_make(1, 1));
        C_cnt = 1;
    }

    while (C_cnt <= N) {
        size_t K = C_cnt;
        fr_t sum = fr_zero();
        for (size_t k = 0; k < K; k++) {
            fr_t ck = fa_get(&Cc, k);
            if (ck.n == 0) continue;
            size_t s = K + 4 - k;
            fr_t H = fr_zero();
            fr_t beta = fr_make(1, 1);   /* binomial(k/2 + i - 1, i) */
            for (size_t i = 0; 2 * i <= s; i++) {
                if (i > 0) {
                    beta = fr_mul(beta, fr_make((long long)k + 2 * (long long)i - 2,
                                                2 * (long long)i));
                    if (beta.n == 0) break;
                }
                H = fr_add(H, fr_mul(beta, fa_get(&Mt, i * M_U + (s - 2 * i))));
            }
            sum = fr_add(sum, fr_mul(ck, H));
        }
        fa_set(&Cc, K, fr_neg(fr_mul(sum, fr_make(1, 4 * (long long)K))));
        C_cnt++;
    }
    return Cc.d[N];
}
