/*
 * A296432  Decimal expansion of ratio-sum for A296284; see Comments.
 * https://oeis.org/A296432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of sum_{k>=1} |A296284(k)/A296284(k-1)
 *            - phi|, phi=(1+sqrt(5))/2; A296284: a(0)=1, a(1)=2,
 *            a(k)=a(k-1)+a(k-2)+k*b(k-2), b = increasing complement of a
 *            (b(0)=3). Ratios r(k)=a(k)/a(k-1) follow
 *            r(k)=1+1/r(k-1)+k*b(k-2)/a(k-1), tracked in binary fixed point
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint32_t rs_u32;
typedef unsigned long long rs_u64;

static int rs_cmpn(const rs_u32 *a, const rs_u32 *b, int n) {
    for (int i = n - 1; i >= 0; i--) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0;
}
static void rs_subn(rs_u32 *a, const rs_u32 *b, int n) {
    rs_u64 br = 0;
    for (int i = 0; i < n; i++) {
        rs_u64 v = (rs_u64)a[i] - (rs_u64)b[i] - br;
        a[i] = (rs_u32)v;
        br = (v >> 63) & 1;
    }
}
static void rs_addn(rs_u32 *a, const rs_u32 *b, int n) {
    rs_u64 c = 0;
    for (int i = 0; i < n; i++) {
        rs_u64 v = (rs_u64)a[i] + b[i] + c;
        a[i] = (rs_u32)v;
        c = v >> 32;
    }
}
static void rs_shl1(rs_u32 *a, int n, rs_u32 bit) {
    rs_u32 c = bit;
    for (int i = 0; i < n; i++) {
        rs_u32 nc = a[i] >> 31;
        a[i] = (a[i] << 1) | c;
        c = nc;
    }
}
static void rs_shr1(rs_u32 *a, int n) {
    for (int i = 0; i < n; i++) {
        rs_u32 hi = (i + 1 < n) ? (a[i + 1] & 1u) : 0u;
        a[i] = (a[i] >> 1) | (hi << 31);
    }
}
/* out = (a*b) >> F ; L limbs used, FL fraction limbs, out has W limbs */
static void rs_mulfix(rs_u32 *out, const rs_u32 *a, const rs_u32 *b, int L, int FL, int W, rs_u32 *tmp) {
    for (int i = 0; i < 2 * L + 1; i++) tmp[i] = 0;
    for (int i = 0; i < L; i++) {
        rs_u64 c = 0;
        if (a[i] == 0) continue;
        for (int j = 0; j < L; j++) {
            rs_u64 v = (rs_u64)a[i] * b[j] + tmp[i + j] + c;
            tmp[i + j] = (rs_u32)v;
            c = v >> 32;
        }
        tmp[i + L] = (rs_u32)((rs_u64)tmp[i + L] + c);
    }
    for (int k = 0; k < W; k++) out[k] = 0;
    for (int k = 0; k < L; k++) out[k] = tmp[k + FL];
}
static void rs_mulsmall(rs_u32 *x, int n, rs_u64 m) {
    rs_u64 c = 0;
    for (int i = 0; i < n; i++) {
        rs_u64 v = (rs_u64)x[i] * m + c;
        x[i] = (rs_u32)v;
        c = v >> 32;
    }
}
/* out = 2^(2F)/R in fixed point (value 1/r) */
static void rs_recip(rs_u32 *out, const rs_u32 *R, int FL, int W, rs_u32 *rem) {
    int F = FL * 32;
    for (int i = 0; i < W; i++) { rem[i] = 0; out[i] = 0; }
    rem[FL] = 1;
    for (int i = F; i >= 0; i--) {
        if (i < F) rs_shl1(rem, W, 0);
        if (rs_cmpn(rem, R, W) >= 0) {
            rs_subn(rem, R, W);
            out[i >> 5] |= (1u << (i & 31));
        }
    }
}

static unsigned char *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    int Bbits = (int)(M * 10 / 3) + 64;
    int FL = (Bbits + 31) / 32 + 2;
    int L = FL + 2;
    int W = 2 * L + 4;
    size_t sz = (size_t)W * sizeof(rs_u32);
    rs_u32 *phi = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *S = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *r = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *inv = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *w = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *e = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *t = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *rem = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *res = (rs_u32 *)calloc(W, sizeof(rs_u32));
    rs_u32 *tmp = (rs_u32 *)calloc(2 * L + 2, sizeof(rs_u32));
    int F = FL * 32;

    /* sqrt(5) * 2^F via binary digit-by-digit square root */
    for (int i = 0; i < F + 2; i++) {
        rs_u32 d = (i < 2) ? 1u : 0u;
        rs_shl1(rem, W, 0);
        rs_shl1(rem, W, d);
        memcpy(t, res, sz);
        rs_shl1(t, W, 0);
        rs_shl1(t, W, 1);
        if (rs_cmpn(rem, t, W) >= 0) {
            rs_subn(rem, t, W);
            rs_shl1(res, W, 1);
        } else {
            rs_shl1(res, W, 0);
        }
    }
    res[FL] += 1;
    rs_shr1(res, W);
    memcpy(phi, res, sz);

    /* b / a bookkeeping */
    const rs_u64 CAP = 1u << 20;
    unsigned char *isa = (unsigned char *)calloc((size_t)CAP + 2, 1);
    size_t bcap = 64, blen = 0;
    rs_u64 *barr = (rs_u64 *)malloc(bcap * sizeof(rs_u64));
    rs_u64 cand = 1;
    rs_u64 p2 = 1, p1 = 2; /* a(n-2), a(n-1) clamped */
    isa[1] = 1; isa[2] = 1;

    memset(r, 0, sz);
    r[FL] = 2;
    memset(w, 0, sz);
    w[FL] = 1;
    int small = 0;
    rs_u64 n = 1;
    for (;; n++) {
        if (n >= 2) {
            rs_u64 k = n - 2;
            while (blen <= k) {
                while (cand <= CAP && isa[cand]) cand++;
                if (blen == bcap) { bcap *= 2; barr = (rs_u64 *)realloc(barr, bcap * sizeof(rs_u64)); }
                barr[blen++] = cand;
                cand++;
            }
            rs_u64 c = n * barr[k];
            memcpy(r, inv, sz);
            r[FL] += 1;
            memcpy(t, w, sz);
            rs_mulsmall(t, L, c);
            rs_addn(r, t, L);
            rs_u64 s = p1 + p2 + c;
            if (s > CAP) s = CAP + 1; else isa[s] = 1;
            p2 = p1; p1 = s;
        }
        memcpy(e, r, sz);
        if (rs_cmpn(r, phi, L) >= 0) rs_subn(e, phi, L);
        else { memcpy(e, phi, sz); rs_subn(e, r, L); }
        rs_addn(S, e, L);
        int zero = 1;
        for (int i = 2; i < L; i++) if (e[i]) { zero = 0; break; }
        if (zero) small++; else small = 0;
        if (small >= 5 && n >= 30) break;
        rs_recip(inv, r, FL, W, rem);
        rs_mulfix(t, w, inv, L, FL, W, tmp);
        memcpy(w, t, sz);
    }

    unsigned char *out = (unsigned char *)malloc(M);
    out[0] = (unsigned char)S[FL];
    rs_u32 *fr = (rs_u32 *)calloc(W, sizeof(rs_u32));
    for (int i = 0; i < FL; i++) fr[i] = S[i];
    for (size_t i = 1; i < M; i++) {
        rs_u64 c = 0;
        for (int j = 0; j < FL; j++) {
            rs_u64 v = (rs_u64)fr[j] * 10 + c;
            fr[j] = (rs_u32)v;
            c = v >> 32;
        }
        out[i] = (unsigned char)c;
    }

    free(fr); free(isa); free(barr);
    free(phi); free(S); free(r); free(inv); free(w); free(e);
    free(t); free(rem); free(res); free(tmp);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 30) M = (size_t)n + 30;
        if (M < 100) M = 100;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
