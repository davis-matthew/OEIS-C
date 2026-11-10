/*
 * A245223  Decimal expansion of inf{f(n,1)}, where f(1,x) = x + 1 and
 *          thereafter f(n,x) = x + 1 if n is in A054385, else f(n,x) = 1/x.
 * https://oeis.org/A245223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   f(1,1) = 2; f(m,1) = f(m-1,1) + 1 if m is in A054385, else
 *            1/f(m-1,1). Members of A054385 are the integers that are not of
 *            the form floor(k*e). sup{f(n,1)} is the largest value reached
 *            just before an inversion, and a(n) is the n-th decimal digit of
 *            1/sup. The code works in exact big rationals and...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 999 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int A245223_len(const uint32_t *x, int cap)
{
    while (cap > 0 && x[cap - 1] == 0) cap--;
    return cap;
}

static int A245223_cmp(const uint32_t *a, int la, const uint32_t *b, int lb)
{
    int i;
    if (la != lb) return la < lb ? -1 : 1;
    for (i = la - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void A245223_mul(const uint32_t *a, int la, const uint32_t *b, int lb, uint32_t *out)
{
    int i, j;
    memset(out, 0, sizeof(uint32_t) * (size_t)(la + lb));
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        for (j = 0; j < lb; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + out[i + j] + carry;
            out[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        out[i + lb] = (uint32_t)carry;
    }
}

static double A245223_ratio(const uint32_t *p, int lp, const uint32_t *q, int lq)
{
    double vp = 0, vq = 0;
    int kp = lp < 3 ? lp : 3, kq = lq < 3 ? lq : 3, i;
    for (i = 0; i < kp; i++) vp = vp * 4294967296.0 + p[lp - 1 - i];
    for (i = 0; i < kq; i++) vq = vq * 4294967296.0 + q[lq - 1 - i];
    return ldexp(vp / vq, 32 * ((lp - kp) - (lq - kq)));
}

static void A245223_run(size_t N, int D, char *out)
{
    size_t cap = N / 16 + 8;
    unsigned char *isB = (unsigned char *)calloc(N + 3, 1); /* isB[m]: m = floor(k*e), i.e. NOT in A054385 */
    uint32_t *P = (uint32_t *)calloc(cap, 4), *Q = (uint32_t *)calloc(cap, 4);
    uint32_t *BP = (uint32_t *)calloc(cap, 4), *BQ = (uint32_t *)calloc(cap, 4);
    uint32_t *T1 = (uint32_t *)calloc(2 * cap, 4), *T2 = (uint32_t *)calloc(2 * cap, 4);
    uint32_t *R = (uint32_t *)calloc(cap + 2, 4);
    uint32_t *p = P, *q = Q, *t;
    int lp, lq, lbp, lbq, tl;
    size_t m, k;
    const unsigned __int128 E = (unsigned __int128)2718281828459045235ULL;
    const unsigned __int128 S = (unsigned __int128)1000000000000000000ULL;

    for (k = 1;; k++) {
        unsigned __int128 v = ((unsigned __int128)k * E) / S;
        if (v > N + 1) break;
        isB[(size_t)v] = 1;
    }
    p[0] = 2; lp = 1;      /* f(1,1) = 2 */
    q[0] = 1; lq = 1;
    BP[0] = 2; lbp = 1;
    BQ[0] = 1; lbq = 1;
    for (m = 2; m <= N; m++) {
        if (isB[m]) {
            t = p; p = q; q = t;
            tl = lp; lp = lq; lq = tl;
        } else {
            int i, n2 = lp > lq ? lp : lq;
            uint64_t c = 0;
            for (i = 0; i < n2; i++) {
                uint64_t s = (uint64_t)(i < lp ? p[i] : 0) + (i < lq ? q[i] : 0) + c;
                p[i] = (uint32_t)s;
                c = s >> 32;
            }
            if (c) { p[n2] = 1; n2++; }
            lp = n2;
        }
        if (m == N || isB[m + 1]) {
            if (A245223_ratio(p, lp, q, lq) >= A245223_ratio(BP, lbp, BQ, lbq) * (1.0 - 1e-9)) {
                A245223_mul(p, lp, BQ, lbq, T1);
                A245223_mul(BP, lbp, q, lq, T2);
                if (A245223_cmp(T1, A245223_len(T1, lp + lbq), T2, A245223_len(T2, lbp + lq)) > 0) {
                    memset(BP, 0, 4 * cap); memset(BQ, 0, 4 * cap);
                    memcpy(BP, p, 4 * (size_t)lp); lbp = lp;
                    memcpy(BQ, q, 4 * (size_t)lq); lbq = lq;
                }
            }
        }
    }
    /* decimal digits of BQ/BP (< 1) */
    memset(R, 0, 4 * (cap + 2));
    memcpy(R, BQ, 4 * (size_t)lbq);
    for (m = 0; m < (size_t)D; m++) {
        int i, lr, d = 0;
        uint64_t c = 0;
        lr = lbp + 1;
        for (i = 0; i < lr; i++) {
            uint64_t s = (uint64_t)R[i] * 10 + c;
            R[i] = (uint32_t)s;
            c = s >> 32;
        }
        lr = A245223_len(R, lr);
        while (A245223_cmp(R, lr, BP, lbp) >= 0) {
            int64_t br = 0;
            for (i = 0; i < lr; i++) {
                int64_t s = (int64_t)R[i] - (int64_t)(i < lbp ? BP[i] : 0) - br;
                if (s < 0) { s += 4294967296LL; br = 1; } else br = 0;
                R[i] = (uint32_t)s;
            }
            lr = A245223_len(R, lr);
            d++;
        }
        out[m] = (char)('0' + d);
    }
    free(isB); free(P); free(Q); free(BP); free(BQ); free(T1); free(T2); free(R);
}

static char *A245223_cache = 0;
static size_t A245223_cached = 0;

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;
    if (nn > A245223_cached) {
        size_t N = 8 * nn + 200;
        int D = (int)nn + 12;
        char *s1 = (char *)malloc((size_t)D + 1);
        char *s2 = (char *)malloc((size_t)D + 1);
        A245223_run(N, D, s1);
        for (;;) {
            N *= 2;
            A245223_run(N, D, s2);
            if (memcmp(s1, s2, (size_t)D) == 0) break;
            { char *tt = s1; s1 = s2; s2 = tt; }
        }
        free(s1);
        free(A245223_cache);
        A245223_cache = s2;
        A245223_cached = (size_t)D - 10;
    }
    return (seq_t)(A245223_cache[nn - 1] - '0');
}
