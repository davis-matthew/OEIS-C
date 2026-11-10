/*
 * A296847  Decimal expansion of ratio-sum for A296846; see Comments.
 * https://oeis.org/A296847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=1} (g -
 *            A296846(k)/A296846(k-1)), where g = (1+sqrt(5))/2 and A296846 is
 *            computed from its recurrence a(n) = a(n-1) + a(n-2) - b(n-2),
 *            a(0)=3, a(1)=5, b(0)=1, b(1)=2, b(2)=4, with b() the complement
 *            of a(). The terms are summed with their signs, as in the OEIS...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t rs_u32;

static int rs_cmp(const rs_u32 *x, const rs_u32 *y, int L) {
    for (int i = L; i > 0; i--) {
        if (x[i - 1] < y[i - 1]) return -1;
        if (x[i - 1] > y[i - 1]) return 1;
    }
    return 0;
}
static void rs_sub(rs_u32 *x, const rs_u32 *y, int L) {
    int64_t br = 0;
    for (int i = 0; i < L; i++) {
        int64_t v = (int64_t)x[i] - (int64_t)y[i] - br;
        if (v < 0) { v += 4294967296LL; br = 1; } else br = 0;
        x[i] = (rs_u32)v;
    }
}
static void rs_add(rs_u32 *x, const rs_u32 *y, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] + y[i] + c;
        x[i] = (rs_u32)v;
        c = v >> 32;
    }
}
static void rs_shl1(rs_u32 *x, int L) {
    rs_u32 c = 0;
    for (int i = 0; i < L; i++) {
        rs_u32 nb = x[i] >> 31;
        x[i] = (x[i] << 1) | c;
        c = nb;
    }
}
static void rs_sub_small(rs_u32 *x, rs_u32 v, int L) {
    int64_t br = v;
    for (int i = 0; i < L && br; i++) {
        int64_t t = (int64_t)x[i] - br;
        if (t < 0) { t += 4294967296LL; br = 1; } else br = 0;
        x[i] = (rs_u32)t;
    }
}
static void rs_mul_small(rs_u32 *x, rs_u32 m, int L) {
    uint64_t c = 0;
    for (int i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (rs_u32)v;
        c = v >> 32;
    }
}
static int rs_len(const rs_u32 *x, int W) {
    int t = W;
    while (t > 1 && !x[t - 1]) t--;
    return t;
}

/* q = floor(num * 2^P / den), q has Q limbs, r is scratch of >= L limbs */
static void rs_ratio(const rs_u32 *num, const rs_u32 *den, int L, int P,
                     rs_u32 *q, int Q, rs_u32 *r) {
    memcpy(r, num, (size_t)L * sizeof(rs_u32));
    memset(q, 0, (size_t)Q * sizeof(rs_u32));
    rs_u32 ip = 0;
    while (rs_cmp(r, den, L) >= 0) { rs_sub(r, den, L); ip++; }
    q[0] = ip;
    for (int i = 0; i < P; i++) {
        rs_shl1(r, L);
        rs_shl1(q, Q);
        if (rs_cmp(r, den, L) >= 0) { rs_sub(r, den, L); q[0] |= 1; }
    }
}

static unsigned char *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    int P = (int)(M * 10 / 3) + 80;
    int Q = P / 32 + 4;
    int Nmax = 5 * P / 2 + 100;
    int W = (2 * P + 256) / 32 + 8;
    int bmax = Nmax + 300;

    rs_u32 *fa = (rs_u32 *)calloc((size_t)W, sizeof(rs_u32));
    rs_u32 *fb = (rs_u32 *)calloc((size_t)W, sizeof(rs_u32));
    rs_u32 *ft = (rs_u32 *)calloc((size_t)W, sizeof(rs_u32));
    rs_u32 *r = (rs_u32 *)calloc((size_t)W + 2, sizeof(rs_u32));
    rs_u32 *phi = (rs_u32 *)calloc((size_t)Q, sizeof(rs_u32));
    rs_u32 *q = (rs_u32 *)calloc((size_t)Q, sizeof(rs_u32));
    rs_u32 *t = (rs_u32 *)calloc((size_t)Q, sizeof(rs_u32));
    rs_u32 *Spos = (rs_u32 *)calloc((size_t)Q, sizeof(rs_u32));
    rs_u32 *Sneg = (rs_u32 *)calloc((size_t)Q, sizeof(rs_u32));

    /* phi via Fibonacci ratio */
    fb[0] = 1;
    int k = (3 * P) / 2 + 20;
    for (int i = 0; i < k; i++) {
        memcpy(ft, fa, (size_t)W * sizeof(rs_u32));
        rs_add(ft, fb, W);
        memcpy(fa, fb, (size_t)W * sizeof(rs_u32));
        memcpy(fb, ft, (size_t)W * sizeof(rs_u32));
    }
    {
        int L = rs_len(fb, W) + 2;
        if (L > W) L = W;
        rs_ratio(fb, fa, L, P, phi, Q, r);
    }

    unsigned char *isA = (unsigned char *)calloc((size_t)bmax, 1);
    uint64_t *barr = (uint64_t *)calloc((size_t)Nmax + 10, sizeof(uint64_t));
    int bcount = 3;
    barr[0] = 1; barr[1] = 2; barr[2] = 4;
    uint64_t blast = 4;
    isA[3] = 1; isA[5] = 1;

    rs_u32 *xm = fa, *x1 = fb, *xn = ft;
    memset(xm, 0, (size_t)W * sizeof(rs_u32));
    memset(x1, 0, (size_t)W * sizeof(rs_u32));
    memset(xn, 0, (size_t)W * sizeof(rs_u32));
    xm[0] = 3;
    x1[0] = 5;

    int zerorun = 0;
    for (int n = 1; n <= Nmax; n++) {
        if (n >= 2) {
            memcpy(xn, x1, (size_t)W * sizeof(rs_u32));
            rs_add(xn, xm, W);
            rs_sub_small(xn, (rs_u32)barr[n - 2], W);
            int small = 1;
            for (int i = 1; i < W; i++) if (xn[i]) { small = 0; break; }
            if (small && xn[0] < (rs_u32)bmax) isA[xn[0]] = 1;
            while (bcount <= n - 1) {
                uint64_t cand = blast + 1;
                while (cand < (uint64_t)bmax && isA[cand]) cand++;
                barr[bcount++] = cand;
                blast = cand;
            }
            rs_u32 *tmp = xm; xm = x1; x1 = xn; xn = tmp;
        }
        int L = rs_len(x1, W) + 2;
        if (L > W) L = W;
        rs_ratio(x1, xm, L, P, q, Q, r);
        /* term = phi - q (signed) */
        int c = rs_cmp(phi, q, Q);
        if (c >= 0) {
            memcpy(t, phi, (size_t)Q * sizeof(rs_u32));
            rs_sub(t, q, Q);
            rs_add(Spos, t, Q);
        } else {
            memcpy(t, q, (size_t)Q * sizeof(rs_u32));
            rs_sub(t, phi, Q);
            rs_add(Sneg, t, Q);
        }
        if (c == 0) { if (++zerorun >= 8) break; } else zerorun = 0;
    }

    /* S = Spos - Sneg (positive result) */
    rs_u32 *S = Spos;
    rs_sub(S, Sneg, Q);

    unsigned char *out = (unsigned char *)malloc(M);
    int pl = P / 32, pb = P % 32;
    for (size_t d = 0; d < M; d++) {
        if (d > 0) rs_mul_small(S, 10, Q);
        unsigned ip = 0;
        for (int j = 0; j < 8; j++) {
            int pos = P + j;
            ip |= ((S[pos / 32] >> (pos % 32)) & 1u) << j;
        }
        S[pl] &= (pb ? ((1u << pb) - 1u) : 0u);
        for (int i = pl + 1; i < Q; i++) S[i] = 0;
        out[d] = (unsigned char)ip;
    }

    free(fa); free(fb); free(ft); free(r); free(phi); free(q); free(t);
    free(Spos); free(Sneg);
    free(isA); free(barr);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = (size_t)n + 20;
        if (M < 100) M = 100;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
