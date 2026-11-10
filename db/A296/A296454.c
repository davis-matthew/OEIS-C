/*
 * A296454  Decimal expansion of limiting power-ratio for A296257; see
 *          Comments.
 * https://oeis.org/A296454
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of L = lim_{m->oo} A296257(m)/phi^m,
 *            phi=(1+sqrt(5))/2; A296257: a(0)=1, a(1)=2,
 *            a(n)=a(n-1)+a(n-2)+b(n-2)^2, b = increasing complement of a,
 *            b(0)=3. Computed as L ~ A(N)*F(N)/F(2N) (phi ~ F(N+1)/F(N)) with
 *            base-1e9 bignums and digit-by-digit long division
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A296454_BASE 1000000000u

static int A296454_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (x[i - 1] < y[i - 1]) return -1;
        if (x[i - 1] > y[i - 1]) return 1;
    }
    return 0;
}

static void A296454_sub(uint32_t *x, const uint32_t *y, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (v < 0) { v += A296454_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
    }
}

static void A296454_add3(uint32_t *out, const uint32_t *x, const uint32_t *y, uint64_t extra, size_t L) {
    uint64_t carry = extra;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)x[i] + y[i] + carry;
        out[i] = (uint32_t)(s % A296454_BASE);
        carry = s / A296454_BASE;
    }
}

static uint8_t *A296454_digs = NULL;
static size_t A296454_have = 0;

static void A296454_compute(size_t T) {
    size_t N = (T + 40) * 48 / 10 + 20;
    size_t LB = N / 20 + 6;

    /* A(N) via recurrence */
    uint32_t *p2 = (uint32_t *)calloc(LB, sizeof(uint32_t));
    uint32_t *p1 = (uint32_t *)calloc(LB, sizeof(uint32_t));
    uint32_t *cu = (uint32_t *)calloc(LB, sizeof(uint32_t));
    p2[0] = 1; p1[0] = 2;
    unsigned long long sa[160];
    size_t sn = 0;
    sa[sn++] = 1; sa[sn++] = 2;
    unsigned long long s2 = 1, s1 = 2;
    const unsigned long long CAP = 1000000000000000000ULL;
    unsigned long long cand = 2;
    for (size_t n = 2; n <= N; n++) {
        int in;
        do {
            cand++;
            in = 0;
            for (size_t j = 0; j < sn; j++) if (sa[j] == cand) { in = 1; break; }
        } while (in);
        unsigned long long bsq = cand * cand;
        A296454_add3(cu, p1, p2, bsq, LB);
        unsigned long long sv = s1 + s2 + bsq;
        if (s1 >= CAP || s2 >= CAP || sv >= CAP) sv = CAP;
        if (sv < CAP && sn < 160) sa[sn++] = sv;
        s2 = s1; s1 = sv;
        uint32_t *t = p2; p2 = p1; p1 = cu; cu = t;
    }
    uint32_t *aN = p1;   /* A(N) */
    free(p2); free(cu);

    /* Fibonacci F(N), F(2N) */
    uint32_t *fx = (uint32_t *)calloc(LB, sizeof(uint32_t));
    uint32_t *fy = (uint32_t *)calloc(LB, sizeof(uint32_t));
    uint32_t *fz = (uint32_t *)calloc(LB, sizeof(uint32_t));
    uint32_t *fN = (uint32_t *)calloc(LB, sizeof(uint32_t));
    fy[0] = 1;
    for (size_t i = 1; i < 2 * N; i++) {
        if (i == N) memcpy(fN, fy, LB * sizeof(uint32_t));
        A296454_add3(fz, fx, fy, 0, LB);
        uint32_t *t = fx; fx = fy; fy = fz; fz = t;
    }
    uint32_t *f2N = fy;
    free(fx); free(fz);

    /* numerator = A(N)*F(N) */
    size_t LP = 2 * LB + 2;
    uint32_t *prod = (uint32_t *)calloc(LP, sizeof(uint32_t));
    for (size_t i = 0; i < LB; i++) {
        if (aN[i] == 0) continue;
        uint64_t carry = 0;
        for (size_t j = 0; j < LB; j++) {
            uint64_t v = (uint64_t)aN[i] * fN[j] + prod[i + j] + carry;
            prod[i + j] = (uint32_t)(v % A296454_BASE);
            carry = v / A296454_BASE;
        }
        size_t k = i + LB;
        while (carry) {
            uint64_t v = (uint64_t)prod[k] + carry;
            prod[k] = (uint32_t)(v % A296454_BASE);
            carry = v / A296454_BASE;
            k++;
        }
    }
    free(aN); free(fN);

    size_t ld = LB;
    while (ld > 0 && f2N[ld - 1] == 0) ld--;
    size_t LL = ld + 2;
    uint32_t *r = (uint32_t *)calloc(LL, sizeof(uint32_t));
    uint32_t *den = (uint32_t *)calloc(LL, sizeof(uint32_t));
    memcpy(den, f2N, ld * sizeof(uint32_t));
    size_t lp = LP < LL ? LP : LL;
    memcpy(r, prod, lp * sizeof(uint32_t));
    free(prod); free(f2N);

    uint8_t *out = (uint8_t *)malloc(T + 8);
    unsigned q0 = 0;
    while (A296454_cmp(r, den, LL) >= 0) { A296454_sub(r, den, LL); q0++; }
    char tmp[16];
    size_t pos = 0;
    {
        int len = 0;
        unsigned v = q0;
        do { tmp[len++] = (char)(v % 10); v /= 10; } while (v);
        while (len > 0 && pos < T) out[pos++] = (uint8_t)tmp[--len];
    }
    while (pos < T) {
        uint64_t carry = 0;
        for (size_t i = 0; i < LL; i++) {
            uint64_t v = (uint64_t)r[i] * 10 + carry;
            r[i] = (uint32_t)(v % A296454_BASE);
            carry = v / A296454_BASE;
        }
        uint8_t d = 0;
        while (A296454_cmp(r, den, LL) >= 0) { A296454_sub(r, den, LL); d++; }
        out[pos++] = d;
    }
    free(r); free(den);
    free(A296454_digs);
    A296454_digs = out;
    A296454_have = T;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    size_t k = (size_t)(n - 2);
    if (k >= A296454_have) {
        size_t T = A296454_have * 2;
        if (T < k + 40) T = k + 40;
        if (T < 64) T = 64;
        A296454_compute(T);
    }
    return (seq_t)A296454_digs[k];
}
