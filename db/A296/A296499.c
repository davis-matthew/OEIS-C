/*
 * A296499  Decimal expansion of ratio-sum for A294546; see Comments.
 * https://oeis.org/A296499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th digit of sum_{k>=1} |A294546(k)/A294546(k-1) - phi|,
 *            phi=(1+sqrt(5))/2; A294546: a(k)=a(k-1)+a(k-2)+b(k-1)+k, a(0)=1,
 *            a(1)=2, b(0)=3, b = complement of a (increasing). Computed with
 *            decimal fixed-point big-integer arithmetic
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

#define SR_BASE 1000000000u

static int sr_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void sr_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint32_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint32_t s = a[i] + b[i] + carry;
        if (s >= SR_BASE) { s -= SR_BASE; carry = 1; } else carry = 0;
        a[i] = s;
    }
}

static void sr_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += SR_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}

static void sr_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % SR_BASE);
        carry = v / SR_BASE;
    }
}

static void sr_add_small(uint32_t *x, size_t L, uint64_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % SR_BASE);
        carry = s / SR_BASE;
    }
}

static size_t sr_digits(const uint32_t *a, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1]) {
            size_t c = 0;
            uint32_t t = a[i - 1];
            while (t) { c++; t /= 10; }
            return 9 * (i - 1) + c;
        }
    }
    return 0;
}

/* Q = floor(X * 10^D / Y); R is scratch */
static void sr_divfix(const uint32_t *X, const uint32_t *Y, uint32_t *Q,
                      uint32_t *R, size_t L, size_t D) {
    memcpy(R, X, L * sizeof(uint32_t));
    memset(Q, 0, L * sizeof(uint32_t));
    while (sr_cmp(R, Y, L) >= 0) {
        sr_sub(R, Y, L);
        sr_add_small(Q, L, 1);
    }
    for (size_t i = 0; i < D; i++) {
        sr_mul_small(R, L, 10);
        uint32_t dg = 0;
        while (sr_cmp(R, Y, L) >= 0) {
            sr_sub(R, Y, L);
            dg++;
        }
        sr_mul_small(Q, L, 10);
        sr_add_small(Q, L, dg);
    }
}

static uint8_t *sr_out = NULL;
static size_t sr_have = 0;

static void sr_compute(size_t M) {
    size_t D = M + 20;
    size_t L = (D + 30) / 9 + 3;
    size_t Kmax = 5 * (D + 20) + 100;
    size_t sz = L * sizeof(uint32_t);
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *C = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *PHI = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *SUM = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *F = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *G = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t *as = (uint64_t *)calloc(Kmax + 2, sizeof(uint64_t));

    /* phi ~ F(m+1)/F(m) */
    F[0] = 1; G[0] = 1;
    while (sr_digits(F, L) < D + 3) {
        uint32_t *T = N;
        memcpy(T, G, sz);
        sr_add(G, F, L);
        memcpy(F, T, sz);
    }
    sr_divfix(G, F, PHI, R, L, D);
    memset(N, 0, sz);

    P[0] = 1; C[0] = 2;
    as[0] = 1; as[1] = 2;
    size_t k = 1;
    uint64_t bprev = 3;
    for (;;) {
        /* term |C/P - phi| */
        sr_divfix(C, P, Q, R, L, D);
        if (sr_cmp(Q, PHI, L) >= 0) {
            sr_sub(Q, PHI, L);
        } else {
            memcpy(R, PHI, sz);
            sr_sub(R, Q, L);
            memcpy(Q, R, sz);
        }
        sr_add(SUM, Q, L);
        if (sr_digits(P, L) > D + 8 || k >= Kmax) break;
        k++;
        uint64_t cand = bprev + 1;
        for (;;) {
            int found = 0;
            for (size_t j = 0; j < k; j++)
                if (as[j] == cand) { found = 1; break; }
            if (!found) break;
            cand++;
        }
        bprev = cand;
        memcpy(N, C, sz);
        sr_add(N, P, L);
        sr_add_small(N, L, bprev + (uint64_t)k);
        memcpy(P, C, sz);
        memcpy(C, N, sz);
        {
            int small = 1;
            for (size_t j = 2; j < L; j++)
                if (C[j]) { small = 0; break; }
            as[k] = small ? (uint64_t)C[1] * SR_BASE + C[0] : UINT64_MAX;
        }
    }

    /* extract decimal digits of SUM (value = 4.737... * 10^D) */
    size_t total = 9 * L;
    uint8_t *out = (uint8_t *)malloc(M);
    size_t start = total - (D + 1);
    static const uint32_t p10[9] = {100000000u, 10000000u, 1000000u, 100000u,
                                    10000u, 1000u, 100u, 10u, 1u};
    for (size_t j = 0; j < M; j++) {
        size_t i = start + j;
        uint32_t limb = SUM[L - 1 - i / 9];
        out[j] = (uint8_t)((limb / p10[i % 9]) % 10);
    }

    free(P); free(C); free(N); free(Q); free(R);
    free(PHI); free(SUM); free(F); free(G); free(as);
    free(sr_out);
    sr_out = out;
    sr_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > sr_have) {
        size_t M = sr_have * 2;
        if (M < (size_t)n + 10) M = (size_t)n + 10;
        if (M < 100) M = 100;
        sr_compute(M);
    }
    return (seq_t)sr_out[n - 1];
}
