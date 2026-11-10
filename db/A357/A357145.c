/*
 * A357145  Decimal expansion of Sum_{n>=1} 1/A003422(n).
 * https://oeis.org/A357145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit of Sum_{k>=1} 1/A003422(k), where A003422(k) =
 *            Sum_{j=0..k-1} j! (computed in binary fixed point with
 *            big-number division, then converted to decimal digits)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Knuth algorithm D: q = floor(u/v); u has m limbs, v has n limbs (v[n-1]!=0), m>=n.
   q has m-n+1 limbs; un has m+1 limbs and vn n limbs as scratch. */
static void A357145_div(uint32_t *q, const uint32_t *u, int m, const uint32_t *v, int n,
                        uint32_t *un, uint32_t *vn) {
    const uint64_t b = 4294967296ULL;
    if (n == 1) {
        uint64_t k = 0;
        for (int j = m - 1; j >= 0; j--) {
            uint64_t cur = k * b + u[j];
            q[j] = (uint32_t)(cur / v[0]);
            k = cur % v[0];
        }
        return;
    }
    int s = 0;
    {
        uint32_t t = v[n - 1];
        while (!(t & 0x80000000u)) { t <<= 1; s++; }
    }
    for (int i = n - 1; i > 0; i--)
        vn[i] = (uint32_t)(((uint64_t)v[i] << s) | ((uint64_t)v[i - 1] >> (32 - s)));
    vn[0] = (uint32_t)((uint64_t)v[0] << s);
    un[m] = (uint32_t)((uint64_t)u[m - 1] >> (32 - s));
    for (int i = m - 1; i > 0; i--)
        un[i] = (uint32_t)(((uint64_t)u[i] << s) | ((uint64_t)u[i - 1] >> (32 - s)));
    un[0] = (uint32_t)((uint64_t)u[0] << s);
    for (int j = m - n; j >= 0; j--) {
        uint64_t num = (uint64_t)un[j + n] * b + un[j + n - 1];
        uint64_t qhat = num / vn[n - 1];
        uint64_t rhat = num - qhat * vn[n - 1];
        while (qhat >= b || qhat * vn[n - 2] > b * rhat + un[j + n - 2]) {
            qhat--;
            rhat += vn[n - 1];
            if (rhat >= b) break;
        }
        int64_t t;
        uint64_t k = 0;
        for (int i = 0; i < n; i++) {
            uint64_t p = qhat * vn[i];
            t = (int64_t)un[i + j] - (int64_t)k - (int64_t)(p & 0xFFFFFFFFULL);
            un[i + j] = (uint32_t)t;
            k = (p >> 32) - (uint64_t)(t >> 32);
        }
        t = (int64_t)un[j + n] - (int64_t)k;
        un[j + n] = (uint32_t)t;
        q[j] = (uint32_t)qhat;
        if (t < 0) {
            q[j]--;
            k = 0;
            for (int i = 0; i < n; i++) {
                uint64_t tt = (uint64_t)un[i + j] + vn[i] + k;
                un[i + j] = (uint32_t)tt;
                k = tt >> 32;
            }
            un[j + n] += (uint32_t)k;
        }
    }
}

static uint8_t *A357145_digs = 0;
static size_t A357145_cap = 0;

static void A357145_build(size_t T) {
    size_t W = (size_t)((T * 34ULL / 10 + 160) / 32 + 1);
    size_t sz = W + 4;
    uint32_t *L = (uint32_t *)calloc(sz, 4);
    uint32_t *F = (uint32_t *)calloc(sz, 4);
    uint32_t *S = (uint32_t *)calloc(sz, 4);
    uint32_t *Q = (uint32_t *)calloc(sz, 4);
    uint32_t *U = (uint32_t *)calloc(sz, 4);
    uint32_t *UN = (uint32_t *)calloc(sz + 2, 4);
    uint32_t *VN = (uint32_t *)calloc(sz, 4);
    int m = (int)W + 1;
    U[W] = 1;
    int ln = 1, fn = 1;
    L[0] = 1; F[0] = 1;
    uint64_t k = 1;
    while (ln <= (int)W) {
        int qn = m - ln + 1;
        for (int i = 0; i < qn; i++) Q[i] = 0;
        A357145_div(Q, U, m, L, ln, UN, VN);
        uint64_t c = 0;
        for (int i = 0; i < m; i++) {
            uint64_t t = (uint64_t)S[i] + (i < qn ? Q[i] : 0) + c;
            S[i] = (uint32_t)t;
            c = t >> 32;
        }
        /* F = k!, L += F */
        c = 0;
        for (int i = 0; i < fn; i++) {
            uint64_t t = (uint64_t)F[i] * k + c;
            F[i] = (uint32_t)t;
            c = t >> 32;
        }
        if (c) F[fn++] = (uint32_t)c;
        c = 0;
        int lim = ln > fn ? ln : fn;
        for (int i = 0; i < lim; i++) {
            uint64_t t = (uint64_t)L[i] + (i < fn ? F[i] : 0) + c;
            L[i] = (uint32_t)t;
            c = t >> 32;
        }
        if (c) L[lim++] = (uint32_t)c;
        ln = lim;
        while (ln > 1 && L[ln - 1] == 0) ln--;
        k++;
    }
    free(A357145_digs);
    A357145_digs = (uint8_t *)malloc(T + 16);
    A357145_digs[0] = (uint8_t)S[W]; /* integer part: 1 */
    size_t pos = 1;
    while (pos < T) {
        uint64_t carry = 0;
        for (size_t i = 0; i < W; i++) {
            uint64_t t = (uint64_t)S[i] * 1000000000ULL + carry;
            S[i] = (uint32_t)t;
            carry = t >> 32;
        }
        for (int d = 8; d >= 0; d--) {
            A357145_digs[pos + (size_t)d] = (uint8_t)(carry % 10);
            carry /= 10;
        }
        pos += 9;
    }
    A357145_cap = T;
    free(L); free(F); free(S); free(Q); free(U); free(UN); free(VN);
}

seq_t a(idx_t n) {
    size_t need = (size_t)n;
    if (need > A357145_cap || !A357145_digs) {
        size_t T = A357145_cap * 2;
        if (T < need) T = need;
        if (T < 128) T = 128;
        A357145_build(T);
    }
    return (seq_t)A357145_digs[need - 1];
}
