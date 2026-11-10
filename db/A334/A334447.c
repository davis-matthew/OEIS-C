/*
 * A334447  Decimal expansion of Product_{k>=1} (1 + 1/A002145(k)^4).
 * https://oeis.org/A334447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Product_{p == 3 mod 4 prime} (1 +
 *            1/p^4). Computed with big fixed-point arithmetic. With B_s =
 *            Prod_{p==3 (4)} (1 - p^-s)^-1, Z_s = Sum_{odd m} m^-s and
 *            beta(s) = Dirichlet beta, one has B_s^2 = Z_s * B_{2s} /
 *            beta(s), and the product equals B_4 / B_8. B_s is set to 1...
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

typedef uint32_t A334447_u32;
typedef uint64_t A334447_u64;

static int A334447_cmp(const A334447_u32 *a, const A334447_u32 *b, int n) {
    int i;
    for (i = n - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static A334447_u32 A334447_add(A334447_u32 *a, const A334447_u32 *b, int n) {
    A334447_u64 c = 0;
    int i;
    for (i = 0; i < n; i++) {
        A334447_u64 t = (A334447_u64)a[i] + b[i] + c;
        a[i] = (A334447_u32)t;
        c = t >> 32;
    }
    return (A334447_u32)c;
}

static A334447_u32 A334447_sub(A334447_u32 *a, const A334447_u32 *b, int n) {
    int64_t br = 0;
    int i;
    for (i = 0; i < n; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        if (t < 0) { t += 4294967296LL; br = 1; } else br = 0;
        a[i] = (A334447_u32)t;
    }
    return (A334447_u32)br;
}

static A334447_u32 A334447_divs(A334447_u32 *a, int n, A334447_u32 d) {
    A334447_u64 r = 0;
    int i;
    for (i = n - 1; i >= 0; i--) {
        A334447_u64 cur = (r << 32) | a[i];
        a[i] = (A334447_u32)(cur / d);
        r = cur % d;
    }
    return (A334447_u32)r;
}

static void A334447_muls(A334447_u32 *a, int n, A334447_u32 m) {
    A334447_u64 c = 0;
    int i;
    for (i = 0; i < n; i++) {
        A334447_u64 t = (A334447_u64)a[i] * m + c;
        a[i] = (A334447_u32)t;
        c = t >> 32;
    }
}

static void A334447_mul(A334447_u32 *r, const A334447_u32 *a, int na, const A334447_u32 *b, int nb) {
    int i, j;
    for (i = 0; i < na + nb; i++) r[i] = 0;
    for (i = 0; i < na; i++) {
        A334447_u64 carry = 0;
        A334447_u64 ai = a[i];
        if (ai == 0) continue;
        for (j = 0; j < nb; j++) {
            A334447_u64 cur = ai * b[j] + r[i + j] + carry;
            r[i + j] = (A334447_u32)cur;
            carry = cur >> 32;
        }
        r[i + nb] = (A334447_u32)carry;
    }
}

/* q = floor(u / v), written into q (nq limbs) -- Knuth algorithm D */
static void A334447_bndiv(A334447_u32 *q, int nq, const A334447_u32 *u, int m, const A334447_u32 *v, int n) {
    int i, j, s = 0;
    A334447_u32 top;
    A334447_u32 *vn, *un;
    for (i = 0; i < nq; i++) q[i] = 0;
    while (n > 0 && v[n - 1] == 0) n--;
    while (m > 0 && u[m - 1] == 0) m--;
    if (n == 0 || m < n) return;
    if (n == 1) {
        A334447_u64 r = 0;
        for (i = m - 1; i >= 0; i--) {
            A334447_u64 cur = (r << 32) | u[i];
            A334447_u64 qq = cur / v[0];
            r = cur % v[0];
            if (i < nq) q[i] = (A334447_u32)qq;
        }
        return;
    }
    top = v[n - 1];
    while (!(top & 0x80000000u)) { top <<= 1; s++; }
    vn = (A334447_u32 *)malloc(sizeof(A334447_u32) * (size_t)n);
    un = (A334447_u32 *)malloc(sizeof(A334447_u32) * (size_t)(m + 1));
    if (s) {
        for (i = n - 1; i > 0; i--) vn[i] = (v[i] << s) | (v[i - 1] >> (32 - s));
        vn[0] = v[0] << s;
        un[m] = u[m - 1] >> (32 - s);
        for (i = m - 1; i > 0; i--) un[i] = (u[i] << s) | (u[i - 1] >> (32 - s));
        un[0] = u[0] << s;
    } else {
        for (i = 0; i < n; i++) vn[i] = v[i];
        for (i = 0; i < m; i++) un[i] = u[i];
        un[m] = 0;
    }
    for (j = m - n; j >= 0; j--) {
        A334447_u64 num = ((A334447_u64)un[j + n] << 32) | un[j + n - 1];
        A334447_u64 qhat = num / vn[n - 1];
        A334447_u64 rhat = num - qhat * vn[n - 1];
        int64_t k = 0, t;
        while (qhat >= 4294967296ULL || qhat * vn[n - 2] > ((rhat << 32) | un[j + n - 2])) {
            qhat--;
            rhat += vn[n - 1];
            if (rhat >= 4294967296ULL) break;
        }
        for (i = 0; i < n; i++) {
            A334447_u64 p = qhat * vn[i];
            t = (int64_t)un[i + j] - k - (int64_t)(p & 0xFFFFFFFFULL);
            un[i + j] = (A334447_u32)t;
            k = (int64_t)(p >> 32) - (t >> 32);
        }
        t = (int64_t)un[j + n] - k;
        un[j + n] = (A334447_u32)t;
        if (t < 0) {
            A334447_u64 c = 0;
            qhat--;
            for (i = 0; i < n; i++) {
                A334447_u64 tt = (A334447_u64)un[i + j] + vn[i] + c;
                un[i + j] = (A334447_u32)tt;
                c = tt >> 32;
            }
            un[j + n] += (A334447_u32)c;
        }
        if (j < nq) q[j] = (A334447_u32)qhat;
    }
    free(vn);
    free(un);
}

/* fixed point: W = L+1 limbs, limb L is the integer part, value = X / 2^(32 L) */
static void A334447_fxmul(A334447_u32 *r, const A334447_u32 *a, const A334447_u32 *b, int L) {
    int W = L + 1, i;
    A334447_u32 *t = (A334447_u32 *)malloc(sizeof(A334447_u32) * (size_t)(2 * W));
    A334447_mul(t, a, W, b, W);
    for (i = 0; i < W; i++) r[i] = t[L + i];
    free(t);
}

static void A334447_fxdiv(A334447_u32 *r, const A334447_u32 *a, const A334447_u32 *b, int L) {
    int W = L + 1, i, NL = 2 * L + 1;
    A334447_u32 *u = (A334447_u32 *)calloc((size_t)NL, sizeof(A334447_u32));
    A334447_u32 *q = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    for (i = 0; i < W; i++) u[L + i] = a[i];
    A334447_bndiv(q, W, u, NL, b, W);
    for (i = 0; i < W; i++) r[i] = q[i];
    free(u);
    free(q);
}

static void A334447_fxsqrt(A334447_u32 *r, const A334447_u32 *a, int L) {
    int W = L + 1, NL = 2 * L + 1, i;
    A334447_u32 *N = (A334447_u32 *)calloc((size_t)NL, sizeof(A334447_u32));
    A334447_u32 *x = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    A334447_u32 *y = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    A334447_u32 *q = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    for (i = 0; i < W; i++) N[L + i] = a[i];
    x[L] = 2;
    for (;;) {
        A334447_bndiv(q, W, N, NL, x, W);
        for (i = 0; i < W; i++) y[i] = x[i];
        A334447_add(y, q, W);
        A334447_divs(y, W, 2);
        if (A334447_cmp(y, x, W) >= 0) break;
        for (i = 0; i < W; i++) x[i] = y[i];
    }
    for (i = 0; i < W; i++) r[i] = x[i];
    free(N); free(x); free(y); free(q);
}

static void A334447_shr(A334447_u32 *a, int n, int bits) {
    int ls = bits / 32, bs = bits % 32, i;
    for (i = 0; i < n; i++) {
        A334447_u32 lo = (i + ls < n) ? a[i + ls] : 0;
        A334447_u32 hi = (i + ls + 1 < n) ? a[i + ls + 1] : 0;
        a[i] = bs ? ((lo >> bs) | (hi << (32 - bs))) : lo;
    }
}

static int A334447_iszero(const A334447_u32 *a, int n) {
    int i;
    for (i = 0; i < n; i++) if (a[i]) return 0;
    return 1;
}

/* A = 1 / m^s */
static void A334447_pw(A334447_u32 *A, int L, A334447_u32 m, int s) {
    int W = L + 1, i, rem = s;
    for (i = 0; i < W; i++) A[i] = 0;
    A[L] = 1;
    if (m < 2) return;
    while (rem > 0) {
        A334447_u64 c = m;
        int e = 1;
        while (e < rem && c * m < 4294967296ULL) { c *= m; e++; }
        A334447_divs(A, W, (A334447_u32)c);
        rem -= e;
    }
}

/* direct sums of odd m: Z = sum m^-s, Bt = sum (-1)^k (2k+1)^-s */
static void A334447_direct(A334447_u32 *Z, A334447_u32 *Bt, int L, int s) {
    int W = L + 1, i, k;
    A334447_u32 *A = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    for (i = 0; i < W; i++) { Z[i] = 0; Bt[i] = 0; }
    for (k = 0;; k++) {
        A334447_pw(A, L, (A334447_u32)(2 * k + 1), s);
        if (A334447_iszero(A, W)) break;
        A334447_add(Z, A, W);
        if (k % 2 == 0) A334447_add(Bt, A, W); else A334447_sub(Bt, A, W);
    }
    free(A);
}

/* Cohen-Villegas-Zagier: X = sum (-1)^k / m_k^s, m_k = 2k+1 (mode 0) or k+1 (mode 1) */
static void A334447_cvz(A334447_u32 *X, int L, int s, int mode) {
    int W = L + 1, i, k;
    int n = (int)((double)(32 * L) / 2.5) + 4;
    int LD = (int)((double)n * 2.6 / 32.0) + 4;
    int LS = LD + W + 2;
    A334447_u32 *xa = (A334447_u32 *)calloc((size_t)LD, sizeof(A334447_u32));
    A334447_u32 *xb = (A334447_u32 *)calloc((size_t)LD, sizeof(A334447_u32));
    A334447_u32 *xt = (A334447_u32 *)calloc((size_t)LD, sizeof(A334447_u32));
    A334447_u32 *U = (A334447_u32 *)calloc((size_t)LD, sizeof(A334447_u32));
    A334447_u32 *be = (A334447_u32 *)calloc((size_t)LD, sizeof(A334447_u32));
    A334447_u32 *A = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    A334447_u32 *prod = (A334447_u32 *)calloc((size_t)(LD + W), sizeof(A334447_u32));
    A334447_u32 *Sp = (A334447_u32 *)calloc((size_t)LS, sizeof(A334447_u32));
    A334447_u32 *Sn = (A334447_u32 *)calloc((size_t)LS, sizeof(A334447_u32));
    A334447_u32 *tmp;
    /* d = x_n, x_0 = 1, x_1 = 3, x_i = 6 x_{i-1} - x_{i-2} */
    xa[0] = 1;  /* x_{i-2} */
    xb[0] = 3;  /* x_{i-1} */
    for (i = 2; i <= n; i++) {
        for (k = 0; k < LD; k++) xt[k] = xb[k];
        A334447_muls(xt, LD, 6);
        A334447_sub(xt, xa, LD);
        tmp = xa; xa = xb; xb = xt; xt = tmp;
    }
    for (k = 0; k < LD; k++) U[k] = xb[k];
    be[0] = 1;
    for (k = 0; k < n; k++) {
        A334447_u32 m = (A334447_u32)(mode ? (k + 1) : (2 * k + 1));
        A334447_sub(U, be, LD);
        A334447_pw(A, L, m, s);
        A334447_mul(prod, U, LD, A, W);
        {
            A334447_u32 *S = (k % 2 == 0) ? Sp : Sn;
            A334447_u64 c = 0;
            for (i = 0; i < LS; i++) {
                A334447_u64 t = (A334447_u64)S[i] + (i < LD + W ? prod[i] : 0) + c;
                S[i] = (A334447_u32)t;
                c = t >> 32;
                if (i >= LD + W && c == 0) break;
            }
        }
        if (k < n - 1) {
            A334447_u64 f = 2ULL * (A334447_u64)(n + k) * (A334447_u64)(n - k);
            A334447_u64 g = (A334447_u64)(2 * k + 1) * (A334447_u64)(k + 1);
            A334447_muls(be, LD, (A334447_u32)f);
            A334447_divs(be, LD, (A334447_u32)g);
        }
    }
    A334447_sub(Sp, Sn, LS);
    A334447_bndiv(X, W, Sp, LS, xb, LD);
    free(xa); free(xb); free(xt); free(U); free(be); free(A); free(prod); free(Sp); free(Sn);
}

static unsigned char *A334447_dig = 0;
static int A334447_T = 0;

static void A334447_compute(int T) {
    int bits = (int)(T * 3.3219281) + 128;
    int L = bits / 32 + 2;
    int W = L + 1;
    int i, t, nl, lv;
    int svals[64];
    A334447_u32 *Bcur, *Bprev, *Z, *Bt, *eta, *tmp, *acc, *P, *tt;
    double stop = (bits + 32) * 0.6310;
    nl = 0;
    svals[0] = 4;
    while ((double)svals[nl] <= stop) { svals[nl + 1] = svals[nl] * 2; nl++; }
    /* svals[0..nl], B at svals[nl] taken as 1 */
    Bcur = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    Bprev = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    Z = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    Bt = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    eta = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    tmp = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    acc = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    P = (A334447_u32 *)calloc((size_t)W, sizeof(A334447_u32));
    Bcur[L] = 1;
    for (lv = nl - 1; lv >= 0; lv--) {
        int s = svals[lv];
        if ((long long)s * 6 >= (long long)bits + 64) {
            A334447_direct(Z, Bt, L, s);
        } else {
            A334447_cvz(Bt, L, s, 0);
            A334447_cvz(eta, L, s, 1);
            /* zeta = eta / (1 - 2^(1-s)) */
            for (i = 0; i < W; i++) { acc[i] = eta[i]; tmp[i] = eta[i]; }
            for (;;) {
                A334447_shr(tmp, W, s - 1);
                if (A334447_iszero(tmp, W)) break;
                A334447_add(acc, tmp, W);
            }
            /* Z = zeta * (1 - 2^-s) */
            for (i = 0; i < W; i++) Z[i] = acc[i];
            A334447_shr(acc, W, s);
            A334447_sub(Z, acc, W);
        }
        /* Bnew = sqrt(Z * Bcur / beta) */
        A334447_fxmul(tmp, Z, Bcur, L);
        A334447_fxdiv(tmp, tmp, Bt, L);
        tt = Bprev; Bprev = Bcur; Bcur = tt;
        A334447_fxsqrt(Bcur, tmp, L);
    }
    A334447_fxdiv(P, Bcur, Bprev, L);
    if (A334447_dig) free(A334447_dig);
    A334447_dig = (unsigned char *)malloc((size_t)T + 1);
    A334447_dig[0] = (unsigned char)P[L];
    for (t = 1; t < T; t++) {
        A334447_u64 carry = 0;
        for (i = 0; i < L; i++) {
            A334447_u64 cur = (A334447_u64)P[i] * 10 + carry;
            P[i] = (A334447_u32)cur;
            carry = cur >> 32;
        }
        A334447_dig[t] = (unsigned char)carry;
    }
    A334447_T = T;
    free(Bcur); free(Bprev); free(Z); free(Bt); free(eta); free(tmp); free(acc); free(P);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if ((A334447_u64)n > (A334447_u64)A334447_T) {
        int T = (int)(n * 2);
        if (T < 256) T = 256;
        A334447_compute(T);
    }
    return (seq_t)A334447_dig[n - 1];
}
