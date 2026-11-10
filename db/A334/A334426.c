/*
 * A334426  Decimal expansion of Product_{k>=1} (1 + 1/A002145(k)^3).
 * https://oeis.org/A334426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   with lambda(s) = Sum_{k>=0} (2k+1)^(-s) =
 *            eta(s)*(2^s-1)/(2^s-2), beta(s) = Sum_{k>=0} (-1)^k (2k+1)^(-s),
 *            A(s) = lambda(s)/beta(s) and Pm(s) = Product_{p == 3 mod 4}
 *            1/(1-p^(-s)), we have Pm(s)^2 = A(s)*Pm(2s) and Product_{k>=1}
 *            (1 + 1/A002145(k)^3) = sqrt(A(3)/Pm(6)). Pm(48) is taken as a
 *            direc...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define A334426_B 1000000000u

static int A334426_F, A334426_M;

static uint32_t *A334426_alloc(void) {
    return (uint32_t *)calloc((size_t)A334426_M, sizeof(uint32_t));
}

static void A334426_copy(uint32_t *d, const uint32_t *s) {
    memcpy(d, s, (size_t)A334426_M * sizeof(uint32_t));
}

static void A334426_setint(uint32_t *a, uint32_t v) {
    memset(a, 0, (size_t)A334426_M * sizeof(uint32_t));
    a[A334426_F] = v;
}

static void A334426_mulsm(uint32_t *a, uint32_t m) {
    uint64_t carry = 0;
    for (int i = 0; i < A334426_M; i++) {
        uint64_t cur = (uint64_t)a[i] * m + carry;
        a[i] = (uint32_t)(cur % A334426_B);
        carry = cur / A334426_B;
    }
}

static void A334426_divsm(uint32_t *a, uint32_t d) {
    uint64_t rem = 0;
    for (int i = A334426_M - 1; i >= 0; i--) {
        uint64_t cur = rem * A334426_B + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void A334426_add(uint32_t *a, const uint32_t *b) {
    uint32_t c = 0;
    for (int i = 0; i < A334426_M; i++) {
        uint32_t s = a[i] + b[i] + c;
        if (s >= A334426_B) { s -= A334426_B; c = 1; } else c = 0;
        a[i] = s;
    }
}

static void A334426_sub(uint32_t *a, const uint32_t *b) {
    uint32_t br = 0;
    for (int i = 0; i < A334426_M; i++) {
        int64_t s = (int64_t)a[i] - (int64_t)b[i] - (int64_t)br;
        if (s < 0) { s += A334426_B; br = 1; } else br = 0;
        a[i] = (uint32_t)s;
    }
}

static int A334426_iszero(const uint32_t *a) {
    for (int i = 0; i < A334426_M; i++) if (a[i]) return 0;
    return 1;
}

static void A334426_fmul(uint32_t *r, const uint32_t *a, const uint32_t *b) {
    int M = A334426_M, F = A334426_F;
    uint32_t *p = (uint32_t *)calloc((size_t)(2 * M + 4), sizeof(uint32_t));
    for (int i = 0; i < M; i++) {
        if (!a[i]) continue;
        uint64_t carry = 0;
        for (int j = 0; j < M; j++) {
            uint64_t cur = (uint64_t)p[i + j] + (uint64_t)a[i] * b[j] + carry;
            p[i + j] = (uint32_t)(cur % A334426_B);
            carry = cur / A334426_B;
        }
        int k = i + M;
        while (carry) {
            uint64_t cur = (uint64_t)p[k] + carry;
            p[k] = (uint32_t)(cur % A334426_B);
            carry = cur / A334426_B;
            k++;
        }
    }
    for (int i = 0; i < M; i++) r[i] = p[i + F];
    free(p);
}

/* y = 1/X (X > 0) by Newton iteration */
static void A334426_rcp(uint32_t *y, const uint32_t *X) {
    int M = A334426_M, F = A334426_F;
    uint32_t *t = A334426_alloc(), *s = A334426_alloc(), *yy = A334426_alloc();
    int h = M - 1;
    while (h > 0 && !X[h]) h--;
    double v = (double)X[h];
    if (h >= 1) v += (double)X[h - 1] / 1e9;
    if (h >= 2) v += (double)X[h - 2] / 1e18;
    double f = 1.0 / v;
    int idx = 2 * F - h - 1;
    for (int c = 0; c < 3; c++, idx--) {
        f *= 1e9;
        double fl = floor(f);
        uint32_t l = (fl >= 1e9) ? A334426_B - 1 : (uint32_t)fl;
        if (idx >= 0 && idx < M) yy[idx] = l;
        f -= (double)l;
    }
    long digits = 12;
    int extra = 2;
    while (digits < 9L * M + 20 || extra-- > 0) {
        A334426_fmul(t, X, yy);
        A334426_setint(s, 2);
        A334426_sub(s, t);
        A334426_fmul(yy, yy, s);
        if (digits < 9L * M + 20) digits *= 2;
    }
    A334426_copy(y, yy);
    free(t); free(s); free(yy);
}

static void A334426_sqrt(uint32_t *x, const uint32_t *A) {
    int F = A334426_F;
    uint32_t *r = A334426_alloc(), *xx = A334426_alloc();
    double v = (double)A[F] + (double)A[F - 1] / 1e9 + (double)A[F - 2] / 1e18;
    double sv = sqrt(v);
    double fl = floor(sv);
    xx[F] = (uint32_t)fl;
    sv -= fl;
    for (int i = F - 1; i >= F - 2; i--) {
        sv *= 1e9;
        double g = floor(sv);
        uint32_t l = (g >= 1e9) ? A334426_B - 1 : (uint32_t)g;
        xx[i] = l;
        sv -= (double)l;
    }
    long digits = 12;
    int extra = 2;
    while (digits < 9L * A334426_M + 20 || extra-- > 0) {
        A334426_rcp(r, xx);
        A334426_fmul(r, A, r);
        A334426_add(xx, r);
        A334426_divsm(xx, 2);
        if (digits < 9L * A334426_M + 20) digits *= 2;
    }
    A334426_copy(x, xx);
    free(r); free(xx);
}

/* res = Sum_{k>=0} (-1)^k / m_k^s, m_k = k+1 (odd==0) or 2k+1 (odd==1) */
static void A334426_alt(uint32_t *res, int s, int odd, uint32_t **w, int nn,
                        const uint32_t *invdn) {
    uint32_t *pos = A334426_alloc(), *neg = A334426_alloc();
    uint32_t *a = A334426_alloc(), *t = A334426_alloc();
    for (int k = 0; k < nn; k++) {
        uint32_t m = odd ? (uint32_t)(2 * k + 1) : (uint32_t)(k + 1);
        A334426_setint(a, 1);
        for (int j = 0; j < s; j++) A334426_divsm(a, m);
        A334426_fmul(t, w[k], a);
        if (k % 2 == 0) A334426_add(pos, t); else A334426_add(neg, t);
    }
    A334426_sub(pos, neg);
    A334426_fmul(res, pos, invdn);
    free(pos); free(neg); free(a); free(t);
}

static int A334426_isp(uint32_t p) {
    if (p < 2) return 0;
    for (uint32_t d = 2; d * d <= p; d++) if (p % d == 0) return 0;
    return 1;
}

static unsigned char *A334426_dig = 0;
static idx_t A334426_have = 0;

static void A334426_compute(idx_t target) {
    int P = (int)target + 40;
    int F = (2 * P + 30) / 9 + 2;
    int I = (P + 20) / 9 + 4;
    int M = F + I;
    A334426_F = F; A334426_M = M;
    int nn = (int)((P + 10) * 1.31) + 6;

    /* Borwein weights w_k = d_n - d_k */
    uint32_t **w = (uint32_t **)malloc((size_t)nn * sizeof(uint32_t *));
    uint32_t *d = A334426_alloc(), *t = A334426_alloc();
    A334426_setint(t, 1);
    A334426_setint(d, 1);
    w[0] = A334426_alloc();
    A334426_copy(w[0], d);
    for (int i = 1; i <= nn; i++) {
        A334426_mulsm(t, (uint32_t)(2 * (nn + i - 1) * (nn - i + 1)));
        A334426_divsm(t, (uint32_t)(i * (2 * i - 1)));
        A334426_add(d, t);
        if (i < nn) { w[i] = A334426_alloc(); A334426_copy(w[i], d); }
    }
    for (int k = 0; k < nn; k++) {
        uint32_t *tmp = A334426_alloc();
        A334426_copy(tmp, d);
        A334426_sub(tmp, w[k]);
        A334426_copy(w[k], tmp);
        free(tmp);
    }
    uint32_t *invdn = A334426_alloc();
    A334426_rcp(invdn, d);

    /* A(s) for s = 3, 6, 12, 24 */
    uint32_t *Av[4];
    int sv[4] = {3, 6, 12, 24};
    uint32_t *eta = A334426_alloc(), *beta = A334426_alloc(), *rb = A334426_alloc();
    for (int q = 0; q < 4; q++) {
        int s = sv[q];
        Av[q] = A334426_alloc();
        A334426_alt(eta, s, 0, w, nn, invdn);
        A334426_alt(beta, s, 1, w, nn, invdn);
        uint32_t ps = (uint32_t)1 << s;
        A334426_mulsm(eta, ps - 1);
        A334426_divsm(eta, ps - 2);
        A334426_rcp(rb, beta);
        A334426_fmul(Av[q], eta, rb);
    }

    /* Pm(48) by direct product over primes p == 3 mod 4 */
    uint32_t *D = A334426_alloc(), *x = A334426_alloc(), *pm = A334426_alloc();
    A334426_setint(D, 1);
    double lim = pow(10.0, (double)(P + 20) / 47.0) + 10.0;
    uint32_t L = (lim > 4.0e9) ? 4000000000u : (uint32_t)lim;
    for (uint32_t p = 3; p <= L; p += 4) {
        if (!A334426_isp(p)) continue;
        A334426_setint(x, 1);
        for (int j = 0; j < 48; j++) A334426_divsm(x, p);
        if (A334426_iszero(x)) break;
        A334426_fmul(t, D, x);
        A334426_sub(D, t);
    }
    A334426_rcp(pm, D);

    /* Pm(s) = sqrt(A(s) * Pm(2s)) for s = 24, 12, 6 */
    for (int q = 3; q >= 1; q--) {
        A334426_fmul(t, Av[q], pm);
        A334426_sqrt(pm, t);
    }
    /* result = sqrt(A(3) / Pm(6)) */
    uint32_t *R = A334426_alloc();
    A334426_rcp(rb, pm);
    A334426_fmul(t, Av[0], rb);
    A334426_sqrt(R, t);

    free(A334426_dig);
    A334426_dig = (unsigned char *)malloc((size_t)target + 1);
    A334426_dig[0] = (unsigned char)(R[F] % 10);
    for (idx_t j = 1; j < target; j++) {
        idx_t q = j - 1;
        int limb = F - 1 - (int)(q / 9);
        int pos = (int)(q % 9);
        uint32_t v = R[limb];
        for (int e = 0; e < 8 - pos; e++) v /= 10;
        A334426_dig[j] = (unsigned char)(v % 10);
    }
    A334426_have = target;

    for (int k = 0; k < nn; k++) free(w[k]);
    free(w);
    for (int q = 0; q < 4; q++) free(Av[q]);
    free(d); free(t); free(invdn); free(eta); free(beta); free(rb);
    free(D); free(x); free(pm); free(R);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n > A334426_have) {
        idx_t target = (n < 110) ? 110 : n + 20;
        A334426_compute(target);
    }
    return (seq_t)A334426_dig[n - 1];
}
