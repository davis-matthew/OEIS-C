/*
 * A334425  Decimal expansion of Product_{k>=1} (1 - 1/A002144(k)^3).
 * https://oeis.org/A334425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   with beta(s) = Sum_{k>=0} (-1)^k/(2k+1)^s, lambda(s) = Sum_{odd
 *            m} 1/m^s and g(s) = Product_{p == 3 mod 4} (1 - p^-s), the Euler
 *            products give a = Product_{p == 1 mod 4} (1 - p^-3) =
 *            1/sqrt(beta(3)*lambda(3)*g(6)), g(s) =
 *            sqrt(beta(s)*g(2s)/lambda(s)), and g(s) -> 1 for large s. beta
 *            and eta (lam...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 106 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define FB 1000000000u

static int gN, gL;

static void fx_zero(uint32_t *a) { for (int i = 0; i < gN; i++) a[i] = 0; }
static void fx_copy(uint32_t *d, const uint32_t *s) { memcpy(d, s, sizeof(uint32_t) * (size_t)gN); }
static int fx_iszero(const uint32_t *a) { for (int i = 0; i < gN; i++) if (a[i]) return 0; return 1; }
static void fx_set_int(uint32_t *a, uint32_t v) { fx_zero(a); a[gL] = v; }

static void fx_add(uint32_t *a, const uint32_t *b) {
    uint32_t c = 0;
    for (int i = 0; i < gN; i++) {
        uint32_t s = a[i] + b[i] + c;
        if (s >= FB) { s -= FB; c = 1; } else c = 0;
        a[i] = s;
    }
}

static void fx_sub(uint32_t *a, const uint32_t *b) {
    int64_t c = 0;
    for (int i = 0; i < gN; i++) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - c;
        if (s < 0) { s += FB; c = 1; } else c = 0;
        a[i] = (uint32_t)s;
    }
}

static void fx_mul_small(uint32_t *a, uint64_t m) {
    uint64_t c = 0;
    for (int i = 0; i < gN; i++) {
        uint64_t cur = (uint64_t)a[i] * m + c;
        a[i] = (uint32_t)(cur % FB);
        c = cur / FB;
    }
}

static void fx_div_small(uint32_t *a, uint64_t d) {
    uint64_t rem = 0;
    for (int i = gN - 1; i >= 0; i--) {
        uint64_t cur = rem * FB + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void fx_mul(uint32_t *out, const uint32_t *a, const uint32_t *b) {
    int N = gN;
    uint32_t *res = (uint32_t *)calloc((size_t)(2 * N + 2), sizeof(uint32_t));
    for (int i = 0; i < N; i++) {
        if (a[i] == 0) continue;
        uint64_t c = 0;
        for (int j = 0; j < N; j++) {
            uint64_t cur = (uint64_t)res[i + j] + (uint64_t)a[i] * b[j] + c;
            res[i + j] = (uint32_t)(cur % FB);
            c = cur / FB;
        }
        int k = i + N;
        while (c) {
            uint64_t cur = (uint64_t)res[k] + c;
            res[k] = (uint32_t)(cur % FB);
            c = cur / FB;
            k++;
        }
    }
    for (int i = 0; i < N; i++) out[i] = res[i + gL];
    free(res);
}

static double fx_to_double(const uint32_t *a) {
    double v = 0.0, s = 1.0;
    for (int i = gL; i < gN; i++) { v += (double)a[i] * s; s *= 1e9; }
    s = 1e-9;
    for (int i = gL - 1; i >= 0 && s > 1e-290; i--) { v += (double)a[i] * s; s /= 1e9; }
    return v;
}

static void fx_from_double(uint32_t *a, double v) {
    fx_zero(a);
    uint64_t ip = (uint64_t)v;
    a[gL] = (uint32_t)(ip % FB);
    a[gL + 1] = (uint32_t)(ip / FB);
    double fr = v - (double)ip;
    for (int i = gL - 1; i >= 0 && i >= gL - 3; i--) {
        fr *= 1e9;
        uint32_t l = (uint32_t)fr;
        if (l >= FB) l = FB - 1;
        a[i] = l;
        fr -= (double)l;
        if (fr < 0) fr = 0;
    }
}

static int fx_iters(void) {
    int it = 3;
    long d = 14;
    while (d < 9L * gL) { d *= 2; it++; }
    return it;
}

static void fx_recip(uint32_t *out, const uint32_t *x) {
    uint32_t *y = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    fx_from_double(y, 1.0 / fx_to_double(x));
    int it = fx_iters();
    for (int i = 0; i < it; i++) {
        fx_mul(t, x, y);
        fx_set_int(u, 2);
        fx_sub(u, t);
        fx_mul(y, y, u);
    }
    fx_copy(out, y);
    free(y); free(t); free(u);
}

static void fx_invsqrt(uint32_t *out, const uint32_t *x) {
    uint32_t *y = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *u = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    double xd = fx_to_double(x), yd = 1.0;
    for (int i = 0; i < 200; i++) yd = yd * (3.0 - xd * yd * yd) / 2.0;
    fx_from_double(y, yd);
    int it = fx_iters();
    for (int i = 0; i < it; i++) {
        fx_mul(t, y, y);
        fx_mul(t, x, t);
        fx_set_int(u, 3);
        fx_sub(u, t);
        fx_mul(y, y, u);
        fx_div_small(y, 2);
    }
    fx_copy(out, y);
    free(y); free(t); free(u);
}

/* a = base^-s */
static void fx_pow_inv(uint32_t *a, uint64_t base, long s) {
    fx_set_int(a, 1);
    long e = s;
    while (e > 0) {
        uint64_t d = 1;
        while (e > 0 && d * base < FB) { d *= base; e--; }
        fx_div_small(a, d);
        if (fx_iszero(a)) return;
    }
}

static uint32_t *gW = 0;
static int gnT = 0;

/* Sum_{k>=0} (-1)^k / (odd ? (2k+1) : (k+1))^s */
static void alt_sum(uint32_t *out, long s, int odd) {
    uint32_t *pos = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *ak = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    for (int k = 0; k < gnT; k++) {
        uint64_t base = odd ? (uint64_t)(2 * k + 1) : (uint64_t)(k + 1);
        fx_pow_inv(ak, base, s);
        if (fx_iszero(ak)) break;
        fx_mul(t, gW + (size_t)k * gN, ak);
        if (k & 1) fx_add(neg, t); else fx_add(pos, t);
    }
    fx_copy(out, pos);
    fx_sub(out, neg);
    free(pos); free(neg); free(ak); free(t);
}

static void lambda_s(uint32_t *out, long s) {
    uint32_t *eta = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *num = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    uint32_t *den = (uint32_t *)calloc((size_t)gN, sizeof(uint32_t));
    alt_sum(eta, s, 0);
    fx_pow_inv(r, 2, s);
    fx_set_int(num, 1); fx_sub(num, r);
    fx_mul_small(r, 2);
    fx_set_int(den, 1); fx_sub(den, r);
    fx_recip(den, den);
    fx_mul(out, eta, num);
    fx_mul(out, out, den);
    free(eta); free(r); free(num); free(den);
}

static uint32_t *cache_f = 0;
static int cache_L = 0;
static long cache_P = 0;

static void compute(long P) {
    int nT = (int)((double)(P + 20) / 0.7655) + 2;
    int E = (int)(((double)nT * 0.765551 - 0.30103) / 9.0);
    if (E < 0) E = 0;
    int L = (int)(P / 9) + 5;
    if (L < E + 4) L = E + 4;
    gL = L; gN = L + 3; gnT = nT;
    int N = gN;

    uint32_t *T = (uint32_t *)calloc((size_t)(nT + 1) * N, sizeof(uint32_t));
    T[L - E] = 1;
    for (int m = 0; m < nT; m++) {
        uint32_t *cur = T + (size_t)m * N, *nx = T + (size_t)(m + 1) * N;
        fx_copy(nx, cur);
        fx_mul_small(nx, 4ull * (uint64_t)(nT + m) * (uint64_t)(nT - m));
        fx_div_small(nx, (uint64_t)(2 * m + 1) * (uint64_t)(2 * m + 2));
    }
    if (gW) free(gW);
    gW = (uint32_t *)calloc((size_t)nT * N, sizeof(uint32_t));
    fx_copy(gW + (size_t)(nT - 1) * N, T + (size_t)nT * N);
    for (int k = nT - 2; k >= 0; k--) {
        fx_copy(gW + (size_t)k * N, gW + (size_t)(k + 1) * N);
        fx_add(gW + (size_t)k * N, T + (size_t)(k + 1) * N);
    }
    uint32_t *D = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    uint32_t *iD = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    fx_copy(D, gW);
    fx_add(D, T);
    fx_recip(iD, D);
    for (int k = 0; k < nT; k++) fx_mul(gW + (size_t)k * N, gW + (size_t)k * N, iD);
    free(T); free(D); free(iD);

    uint32_t *g = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    uint32_t *X = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    uint32_t *b = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    uint32_t *lm = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)N, sizeof(uint32_t));

    long s = 3;
    while ((double)s * 0.4771 < 9.0 * L + 5) s *= 2;
    fx_set_int(g, 1);
    for (long sp = s / 2; sp > 3; sp /= 2) {
        alt_sum(b, sp, 1);
        lambda_s(lm, sp);
        fx_recip(lm, lm);
        fx_mul(X, b, g);
        fx_mul(X, X, lm);
        fx_invsqrt(t, X);
        fx_mul(g, X, t);
    }
    alt_sum(b, 3, 1);
    lambda_s(lm, 3);
    fx_mul(X, b, lm);
    fx_mul(X, X, g);
    fx_invsqrt(t, X);

    if (cache_f) free(cache_f);
    cache_f = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)L);
    for (int i = 0; i < L; i++) cache_f[i] = t[L - 1 - i];
    cache_L = L;
    cache_P = P;

    free(g); free(X); free(b); free(lm); free(t);
}

seq_t a(idx_t n) {
    long need = (long)n + 30;
    if (!cache_f || cache_P < need) {
        long P = need + need / 2 + 100;
        compute(P);
    }
    long j = (long)(n / 9);
    int d = (int)(n % 9);
    uint32_t limb = cache_f[j];
    uint32_t p = 1;
    for (int i = 0; i < 8 - d; i++) p *= 10;
    return (seq_t)((limb / p) % 10);
}
