/*
 * A244646  Decimal expansion of the sum of the reciprocals of the 9-gonal (or
 *          enneagonal or nonagonal) numbers (A001106).
 * https://oeis.org/A244646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th decimal digit of S = Sum_{k>=1} 2/(7k^2-5k) =
 *            (2/5)*(log(14) + (Pi/2)*cot(2Pi/7) - 2*(cos(3Pi/7)*L1 +
 *            cos(Pi/7)*L2 - cos(2Pi/7)*L3)), Lk = -log(sin(k*Pi/7)); computed
 *            with multiprecision fixed-point arithmetic
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 121 terms through n = 967 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int g_L, g_F;
static uint32_t *g_prod = 0;
static unsigned char *g_digs = 0;
static idx_t g_nd = 0;

static uint32_t *vec_new(void) { return (uint32_t *)calloc((size_t)g_L, 4); }
static void vec_set(uint32_t *x, uint32_t v) { memset(x, 0, (size_t)g_L * 4); x[g_F] = v; }
static void vec_cpy(uint32_t *r, const uint32_t *a) { memcpy(r, a, (size_t)g_L * 4); }
static int vec_zero(const uint32_t *a) {
    for (int i = 0; i < g_L; i++) if (a[i]) return 0;
    return 1;
}
static void vec_add(uint32_t *r, const uint32_t *a, const uint32_t *b) {
    uint64_t c = 0;
    for (int i = 0; i < g_L; i++) { c += (uint64_t)a[i] + b[i]; r[i] = (uint32_t)c; c >>= 32; }
}
static void vec_sub(uint32_t *r, const uint32_t *a, const uint32_t *b) {
    uint64_t br = 0;
    for (int i = 0; i < g_L; i++) {
        uint64_t d = (uint64_t)a[i] - b[i] - br;
        r[i] = (uint32_t)d; br = (d >> 32) & 1;
    }
}
static void vec_mul_small(uint32_t *x, uint32_t m) {
    uint64_t c = 0;
    for (int i = 0; i < g_L; i++) { c += (uint64_t)x[i] * m; x[i] = (uint32_t)c; c >>= 32; }
}
static void vec_div_small(uint32_t *x, uint32_t m) {
    uint64_t rem = 0;
    for (int i = g_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / m); rem = cur % m;
    }
}
static void vec_mul(uint32_t *r, const uint32_t *a, const uint32_t *b) {
    int L = g_L, F = g_F;
    memset(g_prod, 0, (size_t)2 * L * 4);
    for (int i = 0; i < L; i++) {
        if (!a[i]) continue;
        uint64_t carry = 0;
        for (int j = 0; j < L; j++) {
            uint64_t cur = (uint64_t)g_prod[i + j] + (uint64_t)a[i] * b[j] + carry;
            g_prod[i + j] = (uint32_t)cur; carry = cur >> 32;
        }
        g_prod[i + L] = (uint32_t)carry;
    }
    for (int k = 0; k < L; k++) r[k] = g_prod[k + F];
}
static void vec_recip(uint32_t *r, const uint32_t *y) {
    int L = g_L, F = g_F;
    double acc = 0;
    for (int i = L - 1; i >= L - 3; i--) acc = acc * 4294967296.0 + (double)y[i];
    double yd = acc / 4294967296.0;
    double r0 = 1.0 / yd;
    memset(r, 0, (size_t)L * 4);
    uint32_t ip = (uint32_t)r0;
    r[F] = ip;
    double f = r0 - (double)ip;
    f *= 4294967296.0; r[F - 1] = (uint32_t)f; f -= (double)r[F - 1];
    f *= 4294967296.0; r[F - 2] = (uint32_t)f;
    uint32_t *e = vec_new(), *t = vec_new();
    for (int it = 0; it < 12; it++) {
        vec_mul(e, y, r);
        vec_set(t, 2);
        vec_sub(t, t, e);
        vec_mul(r, r, t);
    }
    free(e); free(t);
}
/* r = 2*atanh(1/m) */
static void vec_atanh_inv(uint32_t *r, uint32_t m) {
    uint32_t *term = vec_new(), *tmp = vec_new(), *sum = vec_new();
    vec_set(term, 1); vec_div_small(term, m);
    uint32_t m2 = m * m;
    for (uint32_t j = 0; !vec_zero(term); j++) {
        vec_cpy(tmp, term); vec_div_small(tmp, 2 * j + 1);
        vec_add(sum, sum, tmp);
        vec_div_small(term, m2);
    }
    vec_mul_small(sum, 2);
    vec_cpy(r, sum);
    free(term); free(tmp); free(sum);
}
/* r = atan(1/m) */
static void vec_atan_inv(uint32_t *r, uint32_t m) {
    uint32_t *term = vec_new(), *tmp = vec_new(), *pos = vec_new(), *neg = vec_new();
    vec_set(term, 1); vec_div_small(term, m);
    uint32_t m2 = m * m;
    for (uint32_t j = 0; !vec_zero(term); j++) {
        vec_cpy(tmp, term); vec_div_small(tmp, 2 * j + 1);
        if (j % 2 == 0) vec_add(pos, pos, tmp); else vec_add(neg, neg, tmp);
        vec_div_small(term, m2);
    }
    vec_sub(r, pos, neg);
    free(term); free(tmp); free(pos); free(neg);
}
/* l = -ln(y), 0<y<1 */
static void vec_lnneg(uint32_t *l, const uint32_t *y) {
    uint32_t *one = vec_new(), *yp = vec_new(), *inv = vec_new(), *omy = vec_new();
    uint32_t *z = vec_new(), *z2 = vec_new(), *term = vec_new(), *tmp = vec_new(), *sum = vec_new();
    vec_set(one, 1);
    vec_add(yp, one, y);
    vec_recip(inv, yp);
    vec_sub(omy, one, y);
    vec_mul(z, omy, inv);
    vec_mul(z2, z, z);
    vec_cpy(term, z);
    for (uint32_t j = 0; !vec_zero(term); j++) {
        vec_cpy(tmp, term); vec_div_small(tmp, 2 * j + 1);
        vec_add(sum, sum, tmp);
        vec_mul(term, term, z2);
    }
    vec_mul_small(sum, 2);
    vec_cpy(l, sum);
    free(one); free(yp); free(inv); free(omy); free(z); free(z2); free(term); free(tmp); free(sum);
}
static void vec_sincos(uint32_t *c, uint32_t *s, const uint32_t *x) {
    uint32_t *t = vec_new(), *cp = vec_new(), *cn = vec_new(), *sp = vec_new(), *sn = vec_new();
    vec_set(t, 1);
    for (uint32_t k = 0;; k++) {
        if (k > 0) { vec_mul(t, t, x); vec_div_small(t, k); }
        if (k > 4 && vec_zero(t)) break;
        int neg = ((k / 2) % 2) == 1;
        uint32_t *acc;
        if (k % 2 == 0) acc = neg ? cn : cp; else acc = neg ? sn : sp;
        vec_add(acc, acc, t);
    }
    vec_sub(c, cp, cn);
    vec_sub(s, sp, sn);
    free(t); free(cp); free(cn); free(sp); free(sn);
}

static void A244646_compute(idx_t D) {
    free(g_prod); free(g_digs);
    g_F = (int)((D * 3322u) / 1000u / 32u) + 5;
    g_L = g_F + 2;
    g_prod = (uint32_t *)calloc((size_t)2 * g_L, 4);
    uint32_t *a5 = vec_new(), *a239 = vec_new(), *pi = vec_new(), *th = vec_new();
    uint32_t *x = vec_new();
    uint32_t *C1 = vec_new(), *S1 = vec_new(), *C2 = vec_new(), *S2 = vec_new(), *C3 = vec_new(), *S3 = vec_new();
    uint32_t *l1 = vec_new(), *l2 = vec_new(), *l3 = vec_new();
    uint32_t *ln2 = vec_new(), *ln87 = vec_new(), *ln14 = vec_new();
    uint32_t *inv = vec_new(), *cot = vec_new(), *pc = vec_new();
    uint32_t *P = vec_new(), *Q = vec_new(), *tmp = vec_new(), *R = vec_new();

    vec_atan_inv(a5, 5); vec_atan_inv(a239, 239);
    vec_mul_small(a5, 16); vec_mul_small(a239, 4);
    vec_sub(pi, a5, a239);
    vec_cpy(th, pi); vec_div_small(th, 7);

    vec_cpy(x, th); vec_sincos(C1, S1, x);
    vec_mul_small(x, 2); vec_sincos(C2, S2, x);
    vec_cpy(x, th); vec_mul_small(x, 3); vec_sincos(C3, S3, x);

    vec_lnneg(l1, S1); vec_lnneg(l2, S2); vec_lnneg(l3, S3);

    vec_atanh_inv(ln2, 3); vec_atanh_inv(ln87, 15);
    vec_mul_small(ln2, 4);
    vec_sub(ln14, ln2, ln87);

    vec_recip(inv, S2);
    vec_mul(cot, C2, inv);
    vec_mul(pc, pi, cot);
    vec_div_small(pc, 2);

    vec_mul(P, C3, l1);
    vec_mul(tmp, C1, l2);
    vec_add(P, P, tmp);
    vec_mul(Q, C2, l3);

    vec_add(R, ln14, pc);
    vec_add(tmp, Q, Q);
    vec_add(R, R, tmp);
    vec_add(tmp, P, P);
    vec_sub(R, R, tmp);
    vec_mul_small(R, 2);
    vec_div_small(R, 5);

    g_digs = (unsigned char *)calloc((size_t)D + 32, 1);
    g_digs[0] = (unsigned char)R[g_F];
    uint32_t *fr = (uint32_t *)calloc((size_t)g_F, 4);
    memcpy(fr, R, (size_t)g_F * 4);
    idx_t pos = 1;
    while (pos < D + 1) {
        uint64_t carry = 0;
        for (int i = 0; i < g_F; i++) {
            uint64_t cur = (uint64_t)fr[i] * 1000000000u + carry;
            fr[i] = (uint32_t)cur; carry = cur >> 32;
        }
        for (int d = 8; d >= 0; d--) { g_digs[pos + d] = (unsigned char)(carry % 10); carry /= 10; }
        pos += 9;
    }
    free(fr);
    g_nd = D;

    free(a5); free(a239); free(pi); free(th); free(x);
    free(C1); free(S1); free(C2); free(S2); free(C3); free(S3);
    free(l1); free(l2); free(l3); free(ln2); free(ln87); free(ln14);
    free(inv); free(cot); free(pc); free(P); free(Q); free(tmp); free(R);
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (!g_digs || n + 40 > g_nd) {
        idx_t need = n + 40;
        idx_t D = ((need + 511) / 512) * 512;
        if (D < 1024) D = 1024;
        A244646_compute(D);
    }
    return (seq_t)g_digs[n - 1];
}
