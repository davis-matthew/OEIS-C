/*
 * A334445  Decimal expansion of Product_{k>=1} (1 + 1/A002144(k)^4).
 * https://oeis.org/A334445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   with A(s)=Prod_{p==1 mod 4}(1-p^-s)^-1, A(s)^2 = G(s)*A(2s),
 *            where G(s) = zeta(s)(1-2^-s)*beta(s) / (zeta(2s)(1-2^-2s)). The
 *            constant is Product_{k>=1}(1+1/A002144(k)^4) = A(4)/A(8).
 *            Method: fixed-point multiprecision (zeta, beta via
 *            Cohen-Villegas-Zagier acceleration, log/exp series); a(n) =
 *            n-t...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

typedef uint32_t bn_u32;
typedef uint64_t bn_u64;

static int bn_F = 0, bn_L = 0;
static bn_u32 *bn_TMP = 0;
#define BN_SG(a) ((a)[bn_L])

static bn_u32 *bn_new(void) { return (bn_u32 *)calloc((size_t)bn_L + 1, sizeof(bn_u32)); }
static void bn_cp(bn_u32 *r, const bn_u32 *a) { if (r != a) memcpy(r, a, ((size_t)bn_L + 1) * sizeof(bn_u32)); }
static void bn_zero(bn_u32 *r) { memset(r, 0, ((size_t)bn_L + 1) * sizeof(bn_u32)); }
static void bn_setu(bn_u32 *r, bn_u32 v) { bn_zero(r); r[bn_F] = v; }
static int bn_iszero(const bn_u32 *a) {
    for (int i = 0; i < bn_L; i++) if (a[i]) return 0;
    return 1;
}
static int mag_cmp(const bn_u32 *a, const bn_u32 *b) {
    for (int i = bn_L - 1; i >= 0; i--) if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    return 0;
}
static void mag_add(bn_u32 *r, const bn_u32 *a, const bn_u32 *b) {
    bn_u64 c = 0;
    for (int i = 0; i < bn_L; i++) { c += (bn_u64)a[i] + b[i]; r[i] = (bn_u32)c; c >>= 32; }
}
static void mag_sub(bn_u32 *r, const bn_u32 *a, const bn_u32 *b) {
    bn_u64 br = 0;
    for (int i = 0; i < bn_L; i++) {
        bn_u64 x = (bn_u64)a[i] - b[i] - br;
        r[i] = (bn_u32)x;
        br = (x >> 63) & 1;
    }
}
static void bn_addsub(bn_u32 *r, const bn_u32 *a, const bn_u32 *b, int flip) {
    bn_u32 sa = BN_SG(a), sb = BN_SG(b) ^ (bn_u32)flip;
    if (sa == sb) { mag_add(r, a, b); BN_SG(r) = sa; }
    else if (mag_cmp(a, b) >= 0) { mag_sub(r, a, b); BN_SG(r) = sa; }
    else { mag_sub(r, b, a); BN_SG(r) = sb; }
}
static void bn_mul(bn_u32 *r, const bn_u32 *a, const bn_u32 *b) {
    bn_u32 s = BN_SG(a) ^ BN_SG(b);
    int L = bn_L;
    memset(bn_TMP, 0, (size_t)2 * L * sizeof(bn_u32));
    for (int i = 0; i < L; i++) {
        if (!a[i]) continue;
        bn_u64 c = 0;
        for (int j = 0; j < L; j++) {
            bn_u64 cur = (bn_u64)a[i] * b[j] + bn_TMP[i + j] + c;
            bn_TMP[i + j] = (bn_u32)cur;
            c = cur >> 32;
        }
        bn_TMP[i + L] = (bn_u32)c;
    }
    for (int k = 0; k < L; k++) r[k] = bn_TMP[bn_F + k];
    BN_SG(r) = s;
}
static void bn_mul_small(bn_u32 *r, const bn_u32 *a, bn_u32 v) {
    bn_u64 c = 0;
    bn_u32 s = BN_SG(a);
    for (int i = 0; i < bn_L; i++) {
        bn_u64 cur = (bn_u64)a[i] * v + c;
        r[i] = (bn_u32)cur;
        c = cur >> 32;
    }
    BN_SG(r) = s;
}
static void bn_div_small(bn_u32 *r, const bn_u32 *a, bn_u32 v) {
    bn_u64 rem = 0;
    bn_u32 s = BN_SG(a);
    for (int i = bn_L - 1; i >= 0; i--) {
        bn_u64 cur = (rem << 32) | a[i];
        r[i] = (bn_u32)(cur / v);
        rem = cur % v;
    }
    BN_SG(r) = s;
}
static void bn_from_double(bn_u32 *r, double v) {
    bn_zero(r);
    double ip = floor(v);
    r[bn_F] = (bn_u32)ip;
    double fr = v - ip;
    for (int i = bn_F - 1; i >= 0 && i >= bn_F - 3; i--) {
        fr *= 4294967296.0;
        bn_u32 limb = (bn_u32)fr;
        r[i] = limb;
        fr -= (double)limb;
    }
}
static int bn_iters(void) {
    double bits = 32.0 * bn_F;
    int it = 2;
    double p = 40.0;
    while (p < bits) { p *= 2; it++; }
    return it + 1;
}
static void bn_recip(bn_u32 *r, const bn_u32 *a) {
    int lo = bn_F - 3; if (lo < 0) lo = 0;
    double v = 0;
    for (int i = bn_L - 1; i >= lo; i--) v = v * 4294967296.0 + (double)a[i];
    v = ldexp(v, 32 * (lo - bn_F));
    bn_u32 *x = bn_new(), *t = bn_new(), *two = bn_new();
    bn_setu(two, 2);
    bn_from_double(x, 1.0 / v);
    int it = bn_iters();
    for (int k = 0; k < it; k++) {
        bn_mul(t, a, x);
        bn_addsub(t, two, t, 1);
        bn_mul(x, x, t);
    }
    bn_cp(r, x);
    free(x); free(t); free(two);
}
static void bn_pow(bn_u32 *r, const bn_u32 *base, bn_u32 s) {
    bn_u32 *b = bn_new(), *res = bn_new();
    bn_cp(b, base);
    bn_setu(res, 1);
    while (s) {
        if (s & 1) bn_mul(res, res, b);
        s >>= 1;
        if (s) bn_mul(b, b, b);
    }
    bn_cp(r, res);
    free(b); free(res);
}
static void bn_pow2neg(bn_u32 *r, bn_u32 t) {
    bn_zero(r);
    if (t == 0) { r[bn_F] = 1; return; }
    if (t > (bn_u32)(32 * bn_F)) return;
    bn_u32 p = t - 1;
    r[bn_F - 1 - (int)(p / 32)] = 1u << (31 - p % 32);
}
static void bn_sqrt8(bn_u32 *r) {
    bn_u32 *y = bn_new(), *ry = bn_new();
    bn_from_double(y, 2.82842712474619);
    int it = bn_iters();
    for (int k = 0; k < it; k++) {
        bn_recip(ry, y);
        bn_mul_small(ry, ry, 8);
        bn_addsub(y, y, ry, 0);
        bn_div_small(y, y, 2);
    }
    bn_cp(r, y);
    free(y); free(ry);
}

/* sum_{k>=0} (-1)^k / (m*k+c)^s via Cohen-Villegas-Zagier; E = (3-sqrt8)^n */
static void bn_altsum(bn_u32 *res, int m, int c, bn_u32 s, int n, const bn_u32 *E) {
    bn_u32 *bh = bn_new(), *ch = bn_new(), *sum = bn_new(), *a = bn_new();
    bn_u32 *inv = bn_new(), *one = bn_new(), *term = bn_new();
    bn_setu(one, 1);
    bn_cp(bh, E);
    bn_mul_small(bh, bh, 2);
    BN_SG(bh) = 1;
    bn_setu(ch, 1);
    BN_SG(ch) = 1;
    for (int k = 0; k < n; k++) {
        bn_addsub(ch, bh, ch, 1);
        bn_div_small(inv, one, (bn_u32)(m * k + c));
        bn_pow(a, inv, s);
        bn_mul(term, ch, a);
        bn_addsub(sum, sum, term, 0);
        bn_mul_small(bh, bh, (bn_u32)(2 * (k + n)));
        bn_mul_small(bh, bh, (bn_u32)(n - k));
        bn_div_small(bh, bh, (bn_u32)(2 * k + 1));
        bn_div_small(bh, bh, (bn_u32)(k + 1));
        BN_SG(bh) ^= 1;
    }
    bn_cp(res, sum);
    free(bh); free(ch); free(sum); free(a); free(inv); free(one); free(term);
}

/* Zp(s) = zeta(s) * (1 - 2^-s) */
static void bn_zp(bn_u32 *r, bn_u32 s, int n, const bn_u32 *E) {
    bn_u32 *eta = bn_new(), *t1 = bn_new(), *t2 = bn_new(), *one = bn_new(), *p = bn_new();
    bn_setu(one, 1);
    bn_altsum(eta, 1, 1, s, n, E);
    bn_pow2neg(p, s);
    bn_addsub(t1, one, p, 1);
    bn_pow2neg(p, s - 1);
    bn_addsub(t2, one, p, 1);
    bn_recip(t2, t2);
    bn_mul(eta, eta, t1);
    bn_mul(r, eta, t2);
    free(eta); free(t1); free(t2); free(one); free(p);
}

static char *a334445_digs = 0;
static size_t a334445_cnt = 0;

static void a334445_compute(size_t nd) {
    int Dtot = (int)nd + 25;
    int n = (int)(Dtot / 0.766) + 4;
    long bits = (long)(Dtot * 3.3220) + 64 + (long)(2.5445 * n) + 64;
    bn_F = (int)(bits / 32) + 2;
    bn_L = bn_F + 2;
    bn_TMP = (bn_u32 *)malloc((size_t)2 * bn_L * sizeof(bn_u32));

    bn_u32 *s8 = bn_new(), *base = bn_new(), *three = bn_new(), *E = bn_new();
    bn_sqrt8(s8);
    bn_setu(three, 3);
    bn_addsub(base, three, s8, 1);
    bn_pow(E, base, (bn_u32)n);

    int J = 0;
    while (4.0 * (double)(1u << J) * 0.69897 < (double)Dtot + 5.0) J++;

    bn_u32 **Zp = (bn_u32 **)malloc(sizeof(bn_u32 *) * (size_t)(J + 2));
    bn_u32 **Be = (bn_u32 **)malloc(sizeof(bn_u32 *) * (size_t)(J + 1));
    for (int j = 0; j <= J + 1; j++) {
        Zp[j] = bn_new();
        bn_zp(Zp[j], 4u << j, n, E);
    }
    for (int j = 0; j <= J; j++) {
        Be[j] = bn_new();
        bn_altsum(Be[j], 2, 1, 4u << j, n, E);
    }

    bn_u32 *acc = bn_new(), *G = bn_new(), *t = bn_new(), *u = bn_new(), *u2 = bn_new();
    bn_u32 *one = bn_new(), *Lj = bn_new(), *term = bn_new(), *sum = bn_new(), *q = bn_new();
    bn_setu(one, 1);
    for (int j = 0; j <= J; j++) {
        bn_recip(t, Zp[j + 1]);
        bn_mul(G, Zp[j], Be[j]);
        bn_mul(G, G, t);
        bn_addsub(t, G, one, 1);      /* G-1 */
        bn_addsub(u, G, one, 0);      /* G+1 */
        bn_recip(u, u);
        bn_mul(u, t, u);              /* u = (G-1)/(G+1) */
        bn_mul(u2, u, u);
        bn_cp(term, u);
        bn_zero(sum);
        for (int k = 0; k < 100000; k++) {
            if (bn_iszero(term)) break;
            bn_div_small(q, term, (bn_u32)(2 * k + 1));
            bn_addsub(sum, sum, q, 0);
            bn_mul(term, term, u2);
        }
        bn_mul_small(Lj, sum, 2);     /* ln G */
        bn_div_small(Lj, Lj, 1u << (j + 1));
        bn_addsub(acc, acc, Lj, j == 0 ? 0 : 1);
    }
    /* P = exp(acc) */
    bn_u32 *P = bn_new();
    bn_setu(P, 1);
    bn_setu(term, 1);
    for (int k = 1; k < 100000; k++) {
        bn_mul(term, term, acc);
        bn_div_small(term, term, (bn_u32)k);
        if (bn_iszero(term)) break;
        bn_addsub(P, P, term, 0);
    }

    free(a334445_digs);
    a334445_digs = (char *)malloc(nd + 1);
    a334445_digs[0] = (char)P[bn_F];
    P[bn_F] = 0;
    P[bn_F + 1] = 0;
    for (size_t i = 1; i <= nd; i++) {
        bn_mul_small(P, P, 10);
        a334445_digs[i] = (char)P[bn_F];
        P[bn_F] = 0;
        P[bn_F + 1] = 0;
    }
    a334445_cnt = nd + 1;

    for (int j = 0; j <= J + 1; j++) free(Zp[j]);
    for (int j = 0; j <= J; j++) free(Be[j]);
    free(Zp); free(Be);
    free(s8); free(base); free(three); free(E);
    free(acc); free(G); free(t); free(u); free(u2); free(one); free(Lj);
    free(term); free(sum); free(q); free(P);
    free(bn_TMP);
    bn_TMP = 0;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (a334445_digs == 0 || (size_t)n > a334445_cnt) {
        size_t target = (size_t)n + (size_t)n / 2 + 60;
        a334445_compute(target);
    }
    return (seq_t)(unsigned)a334445_digs[n - 1];
}
